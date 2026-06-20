//
// Created by xerox on 6/19/2026.
//

#include "Button.h"

void Button::update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (isHovered()) {
            m_callback();
        }
    }
}

void Button::draw() const {
    m_rect.Draw(LIGHTGRAY);
    const int y = m_rect.y + m_rect.height / 2;
    DrawText(m_text.data(), static_cast<int>(m_rect.x), y, 14, BLACK);

    int lineThick = 1;
    if (isHovered()) {
        lineThick = 3;
    }

    m_rect.DrawLines(DARKGRAY, lineThick);
}

[[nodiscard]] bool Button::isHovered() const {
    return m_rect.CheckCollision(GetMousePosition());
}
