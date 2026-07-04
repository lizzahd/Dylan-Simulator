//
// Created by xerox on 6/28/2026.
//

#pragma once

#include "Character.h"

namespace combat {
    class Player : public Character {
    public:
        Player(ENTITY_REQUIREMENTS, const raylib::Vector2 pos)
            : Character(ENTITY_PARAMETERS, EntityType::Player, pos, 16, 25)
        {}

        void update() override;
        void draw() const override;
        void drawOverlay() const;

        int m_targetEnemy = -1;
        int m_maxWeaponTime = 200; /// Maximum weapon charge time in ticks
        int m_currentWeaponTime = 0; /// Bar moving on weapon charge wheel
        int m_damageThreshold = 30; /// Size of green area on charge wheel
        int m_damageThresholdStart = m_maxWeaponTime / 2 - m_damageThreshold / 2; /// Start of green area on charge wheel
    };
} // combat