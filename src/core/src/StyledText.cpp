//
// Created by xerox on 6/5/2026.
//

#include <StyledText.h>

#include <raylib-cpp.hpp>

#include "GameManager.h"

void drawStyledText(
    const std::shared_ptr<const GameManager> &gameManager,
    Font font,
    const std::string &fullText,
    const raylib::Vector2 pos,
    const float fontSize,
    const float spacing,
    const Color color,
    const int length) {
    std::string text;
    if (length <= 0) {
        text = fullText;
    } else {
        text = fullText.substr(0, length);
    }

    if (font.texture.id == 0) {
        font = GetFontDefault();
    }

    const int textLen = TextLength(text.c_str());

    auto colFront = color;
    auto colBack = BLANK;
    bool isWavy = false;
    const int backRectPadding = 4;

    float textOffsetX = 0.0f;
    float textOffsetY = 0.0f;
    float textLineSpacing = 0.0f;
    const float scaleFactor = fontSize / static_cast<float>(font.baseSize);

    for (int i = 0; i < textLen;) {
        int codepointByteCount = 0;
        const int codepoint = GetCodepointNext(&text[i], &codepointByteCount);

        if (codepoint == '\n') {
            textOffsetX = 0.0f;
            textOffsetY += fontSize + textLineSpacing;
        } else {
            if (codepoint == '[') { // Process pipe styling
                if (i + 2 < textLen && text[i + 1] == 'r' && text[i + 2] == ']') { // Reset styling
                    colFront = color;
                    colBack = BLANK;
                    isWavy = false;

                    i += 3;
                    continue;
                } else if (i + 1 < textLen && (text[i + 1] == 'c' || text[i + 1] == 'b') || text[i + 1] == 'w') {
                    i += 2; // Skip past [c or [b

                    // Parse color
                    const auto sub = text.substr(i);
                    std::string colHexText;
                    int colHexCount = 0;
                    for (const auto &c : sub) {
                        if (c == '\0' || c == ']')
                            break;

                        if ((sub[colHexCount] >= '0' && sub[colHexCount] <= '9') ||
                            (sub[colHexCount] >= 'A' && sub[colHexCount] <= 'F') ||
                            (sub[colHexCount] >= 'a' && sub[colHexCount] <= 'f')) {
                            colHexText += sub[colHexCount];
                            colHexCount++;
                        } else break;
                    }

                    const unsigned int colHexValue = strtoul(colHexText.c_str(), nullptr, 16);
                    switch (text[i - 1]) {
                        case 'b':
                            colBack = GetColor(colHexValue);
                            break;
                        case 'c':
                            colFront = GetColor(colHexValue);
                            break;
                        case 'w':
                            isWavy = true;
                            break;
                        default:
                            break;
                    }

                    i += colHexCount + 1;
                    continue;
                }
            }

            const int index = GetGlyphIndex(font, codepoint);
            float increaseX = 0.0f;

            if (font.glyphs[index].advanceX == 0) {
                increaseX = font.recs[index].width * scaleFactor + spacing;
            } else {
                increaseX += font.recs[index].width * scaleFactor + spacing;
            }

            // Draw background color
            if (colBack.a > 0) {
                DrawRectangleRec({pos.x + textOffsetX, pos.y + textOffsetY - backRectPadding, increaseX, fontSize + backRectPadding * 2}, colBack);
            }

            if (codepoint != ' ' && codepoint != '\t') {
                float waveOffset = 0;
                if (isWavy) {
                    waveOffset = static_cast<float>(std::sin(gameManager->m_time / 5 + static_cast<double>(index) * 45) * 3);
                }
                DrawTextCodepoint(font, codepoint, raylib::Vector2{pos.x + textOffsetX, pos.y + textOffsetY + waveOffset}, fontSize, colFront);
            }

            textOffsetX += increaseX;
        }

        i += codepointByteCount;
    }
}
