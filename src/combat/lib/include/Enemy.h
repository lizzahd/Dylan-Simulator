//
// Created by xerox on 6/28/2026.
//

#pragma once

#include "Character.h"

namespace combat {
    class Enemy : public Character {
    public:
        Enemy(const int id, ILocals *iLocals, const raylib::Vector2 pos)
            : Character(id, iLocals, EntityType::Enemy, pos, 16, 10)
        {}

        void update() override;
        void draw() const override;
        [[nodiscard]] virtual bool canMelee(raylib::Vector2 pos, float radius) const;

        int m_playerId = -1;

        int m_maxAttackCooldown = 50;
        int m_attackCooldown = m_maxAttackCooldown;
    };
}