//
// Created by xerox on 6/28/2026.
//

#include "../include/Player.h"

#include "../include/Enemy.h"
#include "../include/Projectile.h"

#define MOVE_SPEED 2

namespace combat {
    void Player::update() {
        // Input
        const raylib::Vector2 mousePos = GetMousePosition();
        m_angle = std::atan2f(mousePos.y - m_pos.y, mousePos.x - m_pos.x);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_entityManager->create<Projectile>(EntityType::Enemy, m_pos, m_angle, 1, 3);
        }

        if (IsKeyDown(KEY_A)) {
            m_pos.x -= MOVE_SPEED;
        }
        if (IsKeyDown(KEY_D)) {
            m_pos.x += MOVE_SPEED;
        }
        if (IsKeyDown(KEY_W)) {
            m_pos.y -= MOVE_SPEED;
        }
        if (IsKeyDown(KEY_S)) {
            m_pos.y += MOVE_SPEED;
        }

        // Melee Attack
        if (IsKeyPressed(KEY_F)) {
            m_entityManager->breakableExecByType<Character>([&](const int id, auto other) {
                if (other->getType() != EntityType::Enemy) {
                    return false;
                }
                const auto enemy = dynamic_cast<Enemy *>(other);

                if (!enemy->canMelee(m_pos, m_radius)) {
                    return false;
                }

                // TODO: Timing Gauge
                enemy->stun(100);
                m_targetEnemy = id;

                return true;
            });
        }

        if (m_targetEnemy != -1) {
            if (const auto *enemy = m_entityManager->getAs<Enemy>(m_targetEnemy)) {
                if (!enemy->canMelee(m_pos, m_radius)) {
                    m_targetEnemy = -1;
                }
            } else {
                m_targetEnemy = -1;
            }
        }

        Character::update();
    }

    void Player::draw() const {
        m_pos.DrawCircle(m_radius, GREEN);
    }

    void Player::drawOverlay() const {

    }
} // combat