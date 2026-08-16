//
// Created by xerox on 6/28/2026.
//

#include "../include/Character.h"

#include <nlohmann/json.hpp>

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
        const auto sWidth = static_cast<float>(GetScreenWidth());
        const auto sHeight = static_cast<float>(GetScreenHeight());
        if (m_pos.x >= sWidth - m_radius) {
            m_pos.x = sWidth - m_radius;
            m_vel.x *= -0.9f;
        }
        if (m_pos.x <= m_radius) {
            m_pos.x = m_radius;
            m_vel.x *= -0.9f;
        }
        if (m_pos.y >= sHeight - m_radius) {
            m_pos.y = sHeight - m_radius;
            m_vel.y *= -0.9f;
        }
        if (m_pos.y <= m_radius) {
            m_pos.y = m_radius;
            m_vel.y *= -0.9f;
        }
    }

    void Character::hurt(const int damage) {
        if (m_isDead) {
            return;
        }

        json args;
        args["id"] = m_id;
        args["damage"] = damage;
        sig_on_hurt.emit(args);

        m_hp -= damage;

        if (m_hp <= 0) {
            sig_on_death.emit(args);
            m_isDead = true;
        }
    }

    void Character::stun(const int duration) {
        m_stunDuration = duration;
    }
} // combat