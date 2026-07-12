//
// Created by Jehok on 7/11/2026.
//

#pragma once

#include "Animation.h"

// How'd you like my Jarona?

namespace core {
    class PolyAnimation : public Animation {
    public:
        PolyAnimation(
            const std::vector<raylib::Vector2> startPoints,
            const std::vector<raylib::Vector2> endPoints,
            const int maxFrame,
            const int maxFrameTickDelay,
            const bool repeating)
            : Animation(maxFrame, maxFrameTickDelay, repeating)
            , m_startPoints(startPoints)
            , m_endPoints(endPoints)
        {}

        std::vector<raylib::Vector2> m_startPoints;
        std::vector<raylib::Vector2> m_endPoints;


        void draw();
    };
}