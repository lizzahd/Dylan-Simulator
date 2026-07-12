//
// Created by xerox on 5/16/2026.
//

#include <Character.h>
#include <Utils.h>

#include "GameManager.h"
#include "../../gauntlet/include/GauntletCharacter.h"

#define FOLLOW_DIST 32.0f
#define FOLLOW_SPEED 5.0f

namespace core {
    void Character::update() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (isHovered() && canInteract()) {
                m_gameManager->showDialogue(getDialogue());

                if (m_leader == -1) {
                    // Find player
                    m_entityManager->breakableExecByType<Character>([&](const int id, const auto *other) {
                        if (other->m_entityType == EntityType::Player) {
                            follow(id);
                            return true;
                        }

                        return false;
                    });
                } else {
                    stopFollow();
                }
            }
        }

        if (const auto *leader = m_entityManager->getAs<Character>(m_leader)) {
            const float index = static_cast<float>(m_followerIndex);
            if (m_pos.Distance(leader->m_pos) > FOLLOW_DIST * index) {
                const auto targetPos = leader->m_pos + leader->getVector() * (-FOLLOW_DIST * 0.5f * index);
                setAngle(atan2f(targetPos.y - m_pos.y, targetPos.x - m_pos.x));
                if (m_pos.Distance(targetPos) > 5) {
                    const auto d = (targetPos - m_pos).Normalize();
                    m_vel = d * FOLLOW_SPEED;
                    m_direction = leader->m_direction;
                }
            }
        }

        // Check which geometry we're in
        const auto delta = m_pos + m_vel;
        m_fallingVel = FALLING_VELOCITY;
        for (const auto &g : m_map->getGeometries()) {
            if (CheckCollisionPointPoly(m_pos, g.m_vertices.data(), g.m_vertices.size())) {
                // This is the one
                m_fallingVel = 0;

                // Check collision with each line
                for (const auto line : g.m_collisionLines) {
                    line.collideCircle(m_pos, 5);
                }
            }
        }

        if (abs(m_vel.x) > 0.1f || abs(m_vel.y) > 0.1f) {
            m_animationIndex = ANIM_GENERIC_WALK;
            m_animationBank[m_animationIndex].m_playing = true;
        } else {
            m_animationIndex = ANIM_GENERIC_IDLE;
            m_animationBank[m_animationIndex].m_playing = true;
        }

        m_animationBank[m_animationIndex].update();
        m_pos += m_vel;
        m_vel /= 1.5;
    }

    void Character::draw() const {
        const auto &animation = m_animationBank[m_animationIndex];
        int flags = ANIMATION_DRAW_FLAG_NONE;
        AnimationEffectParams effectParams;
        if (isHovered()) {
            flags |= ANIMATION_DRAW_FLAG_OUTLINE;

            effectParams.outlineThickness = 1.0f;
            if (canInteract()) {
                effectParams.outlineColor[0] = 1.0f;
                effectParams.outlineColor[1] = 1.0f;
                effectParams.outlineColor[2] = 1.0f;
                effectParams.outlineColor[3] = 1.0f;
            } else {
                effectParams.outlineColor[0] = 0.5f;
                effectParams.outlineColor[1] = 0.5f;
                effectParams.outlineColor[2] = 0.5f;
                effectParams.outlineColor[3] = 1.0f;
            }
        }
        animation.draw(m_pos, getAngleIndex(), flags, effectParams);
    }

    void Character::drawDebug() const {
        m_pos.DrawLine(m_pos + getVector() * 32, BLUE);
        m_pos.DrawLine(m_pos + m_vel, 2, LIME);
    }

    void Character::switchAnimation(const int animationIndex) {
        m_animationBank[m_animationIndex].reset();
        m_animationIndex = animationIndex;
    }

    void Character::setAngle(const float angle) {
        float degrees = angle * (180.0f / PI);
        if (degrees < 0) {
            degrees += 360.0f;
        }

        const int directionIndex = static_cast<int>((degrees + 22.5f) / 45.0f) % 4;
        // Scale angle to 0-8
        switch (directionIndex) {
            case 0:
                m_direction = Direction::Right;
                break;
            case 1:
                m_direction = Direction::Down;
                break;
            case 2:
                m_direction = Direction::Left;
                break;
            case 3:
                m_direction = Direction::Up;
                break;
            default:
                break;
        }
    }

    float Character::getAngleIndex() const {
        switch (m_direction) {
            case Direction::Right:
                return 0;
            case Direction::Down:
                return 1;
            case Direction::Left:
                return 2;
            case Direction::Up:
                return 3;
        }

        return 0;
    }

    float Character::getAngle() const {
        switch (m_direction) {
            case Direction::Right:
                return 0;
            case Direction::Down:
                return PI_Q * 2;
            case Direction::Left:
                return PI_Q * 4;
            case Direction::Up:
                return PI_Q * 6;
        }

        return 0;
    }

    raylib::Vector2 Character::getVector() const {
        switch (m_direction) {
            case Direction::Right:
                return {1, 0};
            case Direction::Down:
                return {0, 1};
            case Direction::Left:
                return {-1, 0};
            case Direction::Up:
                return {0, -1};
        }

        return {};
    }

    void Character::interact() const {
        m_callback(ENTITY_MEMBERS);
    }

    bool Character::canInteract() const {
        return true; // TODO
    }

    bool Character::isHovered() const {
        return getRect().CheckCollision(GetMousePosition());
    }

    void Character::follow(const int id) {
        if (auto *leader = m_entityManager->getAs<Character>(id)) {
            leader->m_followers.insert(m_id);
            m_followerIndex = static_cast<int>(leader->m_followers.size()) + 1;
            m_leader = id;
        }
    }

    void Character::stopFollow() {
        if (auto *leader = m_entityManager->getAs<Character>(m_leader)) {
            leader->m_followers.erase(m_id);
            for (const auto id : leader->m_followers) {
                if (auto *follower = m_entityManager->getAs<Character>(id)) {
                    follower->follow(m_leader);
                }
            }
        }
        m_leader = -1;
        m_followerIndex = 0;
    }
}
