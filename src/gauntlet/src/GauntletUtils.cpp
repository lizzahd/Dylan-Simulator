//
// Created by xerox on 6/20/2026.
//

#include <GauntletUtils.h>

raylib::Vector2 gauntlet::vectorFromDirection(const Direction direction) {
    switch (direction) {
        case Direction::Up:
            return {0, -1};
        case Direction::Down:
            return {0, 1};
        case Direction::Left:
            return {-1, 0};
        case Direction::Right:
            return {1, 0};
    }

    return {};
}
