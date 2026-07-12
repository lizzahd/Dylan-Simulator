//
// Created by Jehok on 7/11/2026.
//

#include "PolyAnimation.h"
#include <iostream>

namespace core {
    void PolyAnimation::draw() {
        if (m_startPoints.size() != m_endPoints.size()) {
            std::cout << "(PolyAnimation): START POINTS AND END POINTS ARE DIFFERENT LEGTHS!! AAAAAAAAAAHHH";

            exit(-1);
        }

        std::vector<raylib::Vector2> currentPoints;
        const auto currentFrame_F = static_cast<float>(m_currentFrame);
        const auto maxFrame_F = static_cast<float>(m_maxFrame);

        for (std::size_t i = 0; i < m_startPoints.size(); ++i) {
            currentPoints.emplace_back(
                m_startPoints[i].x + ((m_endPoints[i].x-m_startPoints[i].x)/maxFrame_F) * currentFrame_F,
                m_startPoints[i].y + ((m_endPoints[i].y-m_startPoints[i].y)/maxFrame_F) * currentFrame_F
            );
        }

        DrawTriangleFan(currentPoints.data(), static_cast<int>(currentPoints.size()), WHITE);
    }
}
