//
// Created by xerox on 5/22/2026.
//

#include "Animation.h"

#include <raylib-cpp/Rectangle.hpp>

#include "Utils.h"

void Animation::update() {
    if (!m_playing) {
        return;
    }

    if (m_currentFrameTickDelay++ == m_maxFrameTickDelay) {
        m_currentFrameTickDelay = 0;

        if (m_currentFrame++ == m_maxFrame) {
            m_currentFrame = 0;
            if (!m_repeating) {
                m_playing = false;
            }
        }
    }
}

void Animation::draw(const raylib::Vector2 pos, const float ySrcOffset) const {
    const raylib::Rectangle src{static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y};
    const raylib::Rectangle dst{pos, m_size};

    // It's shadow time
    m_assetManager->getTex(m_tex).Draw(
        src,
        raylib::Rectangle{pos.x, pos.y, m_size.x, m_size.y},
        m_origin,
        -60,
        raylib::Color{0, 0, 0, 155}
    );

    m_assetManager->getTex(m_tex).Draw(
        src,
        dst,
        m_origin
    );
}

void Animation::draw(const raylib::Vector2 pos) const {
    draw(pos, 0);
}

void Animation::drawOutline(const raylib::Vector2 pos, const float ySrcOffset, const Color color) const {
    const raylib::Rectangle src{static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y};
    const raylib::Rectangle dst{{pos.x - 4, pos.y - 4}, {m_size.x + 8, m_size.y + 8}};

    m_assetManager->getTex(m_tex).Draw(
        src,
        dst,
        m_origin,
        0,
        color
    );
}

void Animation::reset() {
    m_currentFrame = 0;
    m_currentFrameTickDelay = 0;
}

void Animation::play() {
    reset();
    m_playing = true;
}

raylib::Rectangle Animation::getSourceRect(const float ySrcOffset) const {
    return {static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y};
}
