//
// Created by xerox on 6/28/2026.
//

#include "../include/Projectile.h"
#include "../include/Character.h"

namespace combat {
    void Projectile::update() {
        m_entityManager->breakableExecByType<Character>([&](int, auto other) {
            if (other->getType() != m_target) {
                return false;
            }

            if (CheckCollisionCircles(m_pos, m_radius, other->m_pos, other->m_radius)) {
                other->hurt(m_damage);
                m_isDead = true;
                return true;
            }

            return false;
        });

        m_pos += m_vel;
    }

    void Projectile::draw() const {
        if (m_isDead) {
            return;
        }

        raylib::Color color = WHITE;
        if (m_target == EntityType::Enemy) {
            color = GREEN;
        }

        m_pos.DrawCircle(m_radius, color);
    }
}
