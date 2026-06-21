//
// Created by xerox on 6/20/2026.
//

#pragma once

#include "GauntletCharacter.h"

namespace gauntlet {
    class Player : public Character {
    public:
        Player(ENTITY_REQUIREMENTS, const raylib::Vector2 pos)
            : Character(ENTITY_PARAMETERS, EntityType::Player, pos)
        {}
    };
} // gauntlet