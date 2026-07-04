//
// Created by xerox on 6/28/2026.
//

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "hot_assets/AssetManager.h"

namespace combat {
    void Enemy::update() {
        if (m_stunDuration > 0) {
            m_stunDuration--;
            return;
        }

        // Get Player
        if (m_playerId == -1) {
            m_entityManager->breakableExecByType<Character>([&](const int id, const auto other) {
                if (other->getType() == EntityType::Player) {
                    m_playerId = id;
                    return true;
                }

                return false;
            });
        }

        const auto *player = m_entityManager->getAs<Player>(m_playerId);
        if (!player) {
            return;
        }

        m_angle = std::atan2f(player->m_pos.y - m_pos.y, player->m_pos.x - m_pos.x);

        if (m_attackCooldown-- <= 0) {
            m_entityManager->create<Projectile>(EntityType::Player, m_pos, m_angle, 1, 3);
            m_attackCooldown = m_maxAttackCooldown;
        }

        Character::update();
    }

    void Enemy::draw() const {
        if (m_isDead) {
            return;
        }

        m_pos.DrawCircle(m_radius, WHITE);

        // Aiming thingy
        const float shootDist = m_radius + 5;
        const raylib::Vector2 aimPos = raylib::Vector2{std::cos(m_angle) * shootDist, std::sin(m_angle) * shootDist} + m_pos;
        aimPos.DrawCircle(1, WHITE);

        if (const auto *player = m_entityManager->getAs<Player>(m_playerId)) {
            if (canMelee(player->m_pos, player->m_radius)) {
                DrawText("F", m_pos.x, m_pos.y - m_radius - 20, 16, WHITE);
            }
        }
    }

    [[nodiscard]] bool Enemy::canMelee(const raylib::Vector2 pos, const float radius) const {
        return CheckCollisionCircles(m_pos, m_radius + 20, pos, radius);
    }
}
