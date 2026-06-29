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
    };
} // combat