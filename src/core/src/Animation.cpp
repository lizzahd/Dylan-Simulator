//
// Created by xerox on 5/22/2026.
//

#include "Animation.h"

#include <raylib-cpp/Rectangle.hpp>

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
    m_assetManager->getTex(m_tex).Draw(
        raylib::Rectangle{static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y},
        raylib::Rectangle{pos, m_size},
        m_origin
    );
}

void Animation::draw(const raylib::Vector2 pos) const {
    draw(pos, 0);
}

void Animation::reset() {
    m_currentFrame = 0;
    m_currentFrameTickDelay = 0;
}

void Animation::play() {
    reset();
    m_playing = true;
}
