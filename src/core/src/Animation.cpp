//
// Created by xerox on 5/22/2026.
//

#include "Animation.h"

#include <raylib-cpp/Rectangle.hpp>

#include "Utils.h"

namespace core {
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

    void Animation::reset() {
        m_currentFrame = 0;
        m_currentFrameTickDelay = 0;
    }

    void Animation::play() {
        reset();
        m_playing = true;
    }
}