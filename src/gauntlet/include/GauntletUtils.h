//
// Created by xerox on 6/20/2026.
//

#pragma once

#include <raylib-cpp/Vector2.hpp>

#define TILE_SIZE 32

namespace gauntlet {
    enum class Direction {
        Up,
        Down,
        Left,
        Right,
    };

    raylib::Vector2 vectorFromDirection(Direction direction);
}
