//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <memory>
#include <string>

class GameManager;
struct Font;
struct Color;

namespace raylib {
    class Vector2;
}

void drawStyledText(const std::shared_ptr<const GameManager> &gameManager, Font font, const std::string &fullText, raylib::Vector2 pos, float fontSize, float spacing, Color color, int length);
