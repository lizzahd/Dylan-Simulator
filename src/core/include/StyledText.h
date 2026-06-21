//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <memory>
#include <string>

struct Font;
struct Color;

class GameManager;

namespace raylib {
    class Vector2;
}

namespace core {
    void drawStyledText(
        GameManager *gameManager,
        Font font,
        const std::string &fullText,
        raylib::Vector2 pos,
        float fontSize,
        float spacing,
        Color color,
        int length
    );
}