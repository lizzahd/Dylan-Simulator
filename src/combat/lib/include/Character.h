//
// Created by xerox on 6/28/2026.
//

#pragma once

#include "Actor.h"

namespace combat {
    class Character : public core::Actor {
    public:
        Character(ENTITY_REQUIREMENTS, const EntityType entityType, const raylib::Vector2 pos, const float radius, const int maxHp)
            : Actor(ENTITY_PARAMETERS, EntityBroadType::Character, entityType, pos, {})
            , m_radius(radius)
            , m_maxHp(maxHp)
            , m_hp(maxHp)
        {}

        void update() override;

        [[nodiscard]] bool isDead() const override {
            return m_hp <= 0;
        }

        void hurt(int damage);
        void stun(int duration);

        float m_radius;
        int m_maxHp;
        int m_hp;
        float m_angle = 0;
        int m_stunDuration = 0;
    };
} // combat
