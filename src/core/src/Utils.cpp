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

bool Line::collideCircle(raylib::Vector2 &pos, const float radius) const {
    return collideLineCircle(a, b, pos, radius);
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

bool collideLineCircle(const raylib::Vector2 a, const raylib::Vector2 b, raylib::Vector2 &pos, const float radius) {
    if (a.Distance(pos) < radius) {
        const raylib::Vector2 d = (a - pos).Normalize();
        pos = a - d * radius;
    }

    if (b.Distance(pos) < radius) {
        const raylib::Vector2 d = (b - pos).Normalize();
        pos = b - d * radius;
    }

    auto [dx, dy] = a - b;
    const float len = sqrtf(dx*dx + dy*dy);
    const float dot = (((pos.x - a.x) * (b.x - a.x)) + ((pos.y - a.y) * (b.y - a.y))) / powf(len, 2);
    const raylib::Vector2 closest{
        a.x + (dot * (b.x - a.x)),
        a.y + (dot * (b.y - a.y)),
    };

    if (!CheckCollisionPointLine(closest, a, b, 1)) {
        return false;
    }

    dx = closest.x - pos.x;
    dy = closest.y - pos.y;
    const float dist = sqrtf(dx*dx + dy*dy);

    if (dist <= radius) {
        const raylib::Vector2 d = (closest - pos).Normalize();
        pos = closest - d * radius;
        return true;
    }

    return false;
}
