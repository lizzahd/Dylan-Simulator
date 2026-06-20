//
// Created by xerox on 6/19/2026.
//

#pragma once

#include <functional>
#include <raylib-cpp.hpp>

class Button {
public:
    ~Button() = default;
    Button(const raylib::Rectangle rect, const std::string_view text, const std::function<void()> &callback)
        : m_rect(rect)
        , m_callback(callback)
        , m_text(text)
    {}

    void update();
    void draw() const;
    [[nodiscard]] bool isHovered() const;

    raylib::Rectangle m_rect;
    std::function<void()> m_callback;
    std::string_view m_text;
};
