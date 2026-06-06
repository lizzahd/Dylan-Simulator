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

std::optional<raylib::Vector2> linesCollisionPoint(const raylib::Vector2 pos1, const raylib::Vector2 pos2, const raylib::Vector2 pos3, const raylib::Vector2 pos4) {
    const float uA  = ((pos4.x - pos3.x) * (pos1.y - pos3.y) - (pos4.y - pos3.y) * (pos1.x - pos3.x)) /
                ((pos4.y - pos3.y) * (pos2.x - pos1.x) - (pos4.x - pos3.x) * (pos2.y - pos1.y));
    const float uB  = ((pos2.x - pos1.x) * (pos1.y - pos3.y) - (pos2.y - pos1.y) * (pos1.x - pos3.x)) /
                ((pos4.y - pos3.y) * (pos2.x - pos1.x) - (pos4.x - pos3.x) * (pos2.y - pos1.y));

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        return raylib::Vector2{pos1.x + (uA * (pos2.x - pos1.x)), pos1.y + (uA * (pos2.y - pos1.y))};
    }

    return std::nullopt;
}

bool isInteractKeyPressed() {
    return IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}