//
// Created by xerox on 6/20/2026.
//

#pragma once

#include "GauntletUtils.h"
#include "Actor.h"
#include "Animation.h"

namespace gauntlet {
    class Character : public core::Actor {
    public:
        explicit Character(ENTITY_REQUIREMENTS, const EntityType entityType, const raylib::Vector2 pos)
            : Actor(ENTITY_PARAMETERS, EntityBroadType::Character, entityType, pos, {TILE_SIZE, TILE_SIZE})
        {}

        void update() override;
        void draw() const override;

        Direction m_direction = Direction::Down;
        int m_animationIndex = 0;
        std::vector<core::Animation> m_animationBank;
    };

} // gauntlet