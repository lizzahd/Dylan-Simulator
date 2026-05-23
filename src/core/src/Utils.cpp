//
// Created by xerox on 4/26/2026.
//

#include <Utils.h>

bool lineRecCollision(const raylib::Vector2 pos1, const raylib::Vector2 pos2, const raylib::Rectangle rec) {
    const bool left    = CheckCollisionLines(pos1, pos2, rec.GetPosition(), {rec.x, rec.y + rec.height}, nullptr);
    const bool right   = CheckCollisionLines(pos1, pos2, {rec.x + rec.width, rec.y}, {rec.x + rec.width, rec.y + rec.height}, nullptr);
    const bool top     = CheckCollisionLines(pos1, pos2, rec.GetPosition(), {rec.x + rec.width, rec.y}, nullptr);
    const bool bottom  = CheckCollisionLines(pos1, pos2, {rec.x, rec.y + rec.height}, rec.GetPosition() + rec.GetSize(), nullptr); // NOLINT

    return left || right || top || bottom;
}