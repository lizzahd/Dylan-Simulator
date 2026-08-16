//
// Created by xerox on 6/28/2026.
//

#include "../include/Player.h"

#include <iostream>
#include <nlohmann/json.hpp>

#include "../include/Enemy.h"
#include "../include/Projectile.h"

#define MOVE_SPEED 2

namespace combat {
    raylib::Color get_character_color(const PlayerCharacter &character) {
        switch (character) {
            case PlayerCharacter::Dylan:
                return GREEN;
            case PlayerCharacter::Dorian:
                return BLUE;
            case PlayerCharacter::Dubi:
                return PINK;
            case PlayerCharacter::Mauzer:
                return WHITE;
            default:
                return WHITE;
        }
    }

    void Player::update() {
        // Input
        const raylib::Vector2 mousePos = GetMousePosition();
        m_angle = std::atan2f(mousePos.y - m_pos.y, mousePos.x - m_pos.x);

        if (IsKeyPressed(KEY_TAB)) {
            m_currentCharacter++;
            if (m_currentCharacter >= m_characters.size()) {
                m_currentCharacter = 0;
            }
        }

        switch (m_characters[m_currentCharacter]) {
            case PlayerCharacter::Dylan:
            case PlayerCharacter::Dorian: {
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
                        if (other->getType() != static_cast<int>(EntityType::Enemy)) {
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
                break;
            }
            case PlayerCharacter::Dubi: {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    m_chargeUp += 0.5f;
                    if (m_chargeUp > m_maxChargeUp) {
                        m_chargeUp = m_maxChargeUp;
                    }
                } else {
                    if (m_chargeUp > 0.0f) {
                        // Launch!
                        m_vel = raylib::Vector2(cos(m_angle) * m_chargeUp, sin(m_angle) * m_chargeUp);
                    }
                    m_chargeUp = 0.0f;
                }

                // Detect collision on enemies
                m_entityManager->breakableExecByType<Character>([&](int, auto other) {
                    if (other->m_entityType != EntityType::Enemy || other->isDead()) {
                        return false;
                    }

                    const float totalRadius = m_radius + other->m_radius;
                    if (m_pos.Distance(other->m_pos) <= totalRadius) {
                        // Bounce
                        const raylib::Vector2 d = (other->m_pos - m_pos) / 2.0f;
                        other->m_vel += d;
                        other->hurt(static_cast<int>((abs(m_vel.x) + abs(m_vel.y)) / 5.0f));
                        m_vel -= d;
                    }

                    return false;
                });

                break;
            }
            case PlayerCharacter::Mauzer:
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    m_entityManager->create<Projectile>(EntityType::Enemy, m_pos, m_angle, 1, 3);
                }
                break;
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

        if (m_characters[m_currentCharacter] == PlayerCharacter::Dubi) {
            m_vel /= 1.01;
        } else {
            m_vel /= 1.08;
        }
        m_pos += m_vel;
    }

    void Player::draw() const {
        m_pos.DrawCircle(m_radius, get_character_color(m_characters[m_currentCharacter]));

        // Draw dubi charge-up
        if (m_chargeUp > 0.0f) {
            const float c = cos(m_angle);
            const float s = sin(m_angle);
            const raylib::Vector2 lineStart = m_pos + raylib::Vector2(c * (m_radius + 3), s * (m_radius + 3));
            const raylib::Vector2 lineEnd = lineStart + raylib::Vector2(c * m_chargeUp * 3, s * m_chargeUp * 3);
            lineStart.DrawLine(lineEnd, 2, PINK);
        }
    }

    void Player::drawOverlay() const {

    }

    void Player::on_character_hurt(const json &args) {
        if (args["id"] != m_id) {
            return;
        }

        // Spawn projectiles where the amount is based on damage dealt
        const auto radian = PI * 2 / static_cast<float>(args["damage"]);
        float current_angle = 0;
        for (int i = 0; i < args["damage"]; i++) {
            m_entityManager->create<Projectile>(EntityType::Enemy, m_pos, current_angle, 1, 3);
            current_angle += radian;
        }
    }

    void Player::on_character_death(const json &args) const {
        if (args["id"] == m_id) {
            return;
        }

        // Get entity
        const auto *character = m_entityManager->getAs<Character>(args["id"]);

        // Spawn projectiles where the amount is based on character max health
        const auto radian = PI * 2 / static_cast<float>(character->m_maxHp);
        float current_angle = 0;
        for (int i = 0; i < character->m_maxHp; i++) {
            m_entityManager->create<Projectile>(EntityType::Enemy, character->m_pos, current_angle, 1, 3);
            current_angle += radian;
        }
    }
} // combat