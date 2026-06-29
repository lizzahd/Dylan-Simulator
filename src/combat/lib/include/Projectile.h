//
// Created by xerox on 6/28/2026.
//

#pragma once
#include "Actor.h"

namespace combat {
    class Projectile : public core::Actor {
    public:
        Projectile(ENTITY_REQUIREMENTS,
            const EntityType target,
            const raylib::Vector2 pos,
            const float angle,
            const int damage,
            const float radius)
                : Actor(ENTITY_PARAMETERS, EntityBroadType::Projectile, EntityType::Projectile, pos, {})
                , m_target(target)
                , m_angle(angle)
                , m_damage(damage)
                , m_radius(radius) {
            m_vel = raylib::Vector2{std::cos(angle) * 5, std::sin(angle) * 5};
        }

        void update() override;
        void draw() const override;

        EntityType m_target;
        raylib::Vector2 m_vel;
        float m_angle;
        int m_damage;
        float m_radius;
    };
}
