//
// Created by xerox on 6/28/2026.
//

#include "../include/Character.h"

namespace combat {
    void Character::update() {
        // Character-Character collision
        m_entityManager->execByType<Character>([&](int, const auto other) {
            if (m_pos.Distance(other->m_pos) < m_radius + other->m_radius) {
                const raylib::Vector2 d = (other->m_pos - m_pos).Normalize();
                m_pos -= d;
            }
        });

        // Character-Level collision
        // TODO
    }

    void Character::hurt(const int damage) {
        m_hp -= damage;

        if (m_hp <= 0) {
            m_isDead = true;
        }
    }

    void Character::stun(const int duration) {
        m_stunDuration = duration;
    }
} // combat