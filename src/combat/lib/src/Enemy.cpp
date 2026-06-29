//
// Created by xerox on 6/28/2026.
//

#include "Enemy.h"
#include "Player.h"
#include "Projectile.h"
#include "hot_assets/AssetManager.h"

namespace combat {
    void Enemy::update() {
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
        // const raylib::Rectangle aimSrc{0, 0, 5, 7};
        // const raylib::Rectangle aimDst{aimPos, {5, 7}};
        // m_assetManager->getTex("aim_triangle").Draw(aimSrc, aimDst, {2, 3}, m_angle, WHITE);
    }
}
