//
// Created by xerox on 3/29/2026.
//

#pragma once

#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Vector2.hpp>

#define PI_Q (PI / 4)
#define PI_H (PI / 2)
#define PI_2 (PI * 2)

#define CHUNK_SIZE 64
#define CHUNK_SIZE_H (CHUNK_SIZE / 2)

static bool inDebugMode = false;

inline raylib::Vector2 snapToChunk(const float x, const float y) {
    const int ix = static_cast<int>(x);
    const int iy = static_cast<int>(y);
    return Vector2(static_cast<float>((ix / CHUNK_SIZE) * CHUNK_SIZE), static_cast<float>((iy / CHUNK_SIZE) * CHUNK_SIZE));
}

inline raylib::Vector2 snapToChunk(const Vector2 pos) {
    return snapToChunk(pos.x, pos.y);
}

bool lineRecCollision(raylib::Vector2 pos1, raylib::Vector2 pos2, raylib::Rectangle rec);

// Integer position helper struct
struct IPos {
    int x;
    int y;

    bool operator==(const IPos& rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    [[nodiscard]] raylib::Vector2 toVector() const {
        return {static_cast<float>(x), static_cast<float>(y)};
    }
};

inline IPos vector2ToChunk(const raylib::Vector2 pos) {
    return IPos{static_cast<int>(pos.x) / CHUNK_SIZE, static_cast<int>(pos.y) / CHUNK_SIZE};
}

constexpr IPos ADJACENT_4[4] = {
    {1, 0},     // Right
    {0, 1},     // Bottom
    {-1, 0},    // Left
    {0, -1},    // Top
};

constexpr IPos ADJACENT_8[8] = {
    {1, 0},     // Right
    {1, 1},     // Bottom-Right
    {0, 1},     // Bottom
    {-1, 1},    // Bottom-Left
    {-1, 0},    // Left
    {-1, -1},   // Top-Left
    {0, -1},    // Top
    {1, -1},    // Top-Right
};