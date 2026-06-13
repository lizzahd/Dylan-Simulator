//
// Created by xerox on 5/16/2026.
//

#include <Character.h>
#include <Utils.h>

#include "GameManager.h"

void Character::update() {
    // Check which geometry we're in
    const auto delta = m_pos + m_vel;
    m_fallingVel = FALLING_VELOCITY;
    for (const auto &g : m_map->getGeometries()) {
        if (CheckCollisionPointPoly(m_pos, g.m_vertices.data(), g.m_vertices.size())) {
            // This is the one
            m_fallingVel = 0;

            // Check collision with each line
            for (const auto &[a, b] : g.m_collisionLines) {
                const auto result = linesCollisionPoint(m_pos, delta, a, b);
                if (result.has_value()) {
                    m_vel = 0;
                }
            }
        }
    }

    if (m_fallingVel > 0) {
        m_pos.y += m_fallingVel;
        m_vel = 0;
    } else {
        m_animationBank[m_animationIndex].update();
        m_pos += m_vel;
        // m_vel /= 1.15;
        m_vel /= 1.5;
    }
}

void Character::draw() const {
    const auto &animation = m_animationBank[m_animationIndex];
    animation.draw(m_pos, getAngleIndex());
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

    const int directionIndex = static_cast<int>((degrees + 22.5f) / 45.0f) % 8;
    // Scale angle to 0-8
    switch (directionIndex) {
        case 0:
            m_direction = Direction::Right;
            break;
        case 1:
            m_direction = Direction::DownRight;
            break;
        case 2:
            m_direction = Direction::Down;
            break;
        case 3:
            m_direction = Direction::DownLeft;
            break;
        case 4:
            m_direction = Direction::Left;
            break;
        case 5:
            m_direction = Direction::UpLeft;
            break;
        case 6:
            m_direction = Direction::Up;
            break;
        case 7:
            m_direction = Direction::UpRight;
            break;
        default:
            break;
    }
}

float Character::getAngleIndex() const {
    switch (m_direction) {
        case Direction::Right:
            return 0;
        case Direction::DownRight:
            return 1;
        case Direction::Down:
            return 2;
        case Direction::DownLeft:
            return 3;
        case Direction::Left:
            return 4;
        case Direction::UpLeft:
            return 5;
        case Direction::Up:
            return 6;
        case Direction::UpRight:
            return 7;
    }

    return 0;
}

float Character::getAngle() const {
    switch (m_direction) {
        case Direction::Right:
            return 0;
        case Direction::DownRight:
            return PI_Q;
        case Direction::Down:
            return PI_Q * 2;
        case Direction::DownLeft:
            return PI_Q * 3;
        case Direction::Left:
            return PI_Q * 4;
        case Direction::UpLeft:
            return PI_Q * 5;
        case Direction::Up:
            return PI_Q * 6;
        case Direction::UpRight:
            return PI_Q * 7;
    }

    return 0;
}

raylib::Vector2 Character::getVector() const {
    switch (m_direction) {
        case Direction::Right:
            return {1, 0};
        case Direction::DownRight:
            return {1, 1};
        case Direction::Down:
            return {0, 1};
        case Direction::DownLeft:
            return {-1, 1};
        case Direction::Left:
            return {-1, 0};
        case Direction::UpLeft:
            return {-1, -1};
        case Direction::Up:
            return {0, -1};
        case Direction::UpRight:
            return {1, -1};
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
