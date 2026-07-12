//
// Created by Jehok on 6/28/2026.
//

#pragma once

#include <raylib-cpp/raylib-cpp.hpp>
#include <hot_assets/AssetManager.h>


namespace combat {
    class UI {
    public:
        UI();
        ~UI() = default;

        const int m_AGGRO_DECEL_RATE = 1;

        int m_aggroState;
        int m_rotaryPositon;
        raylib::Vector2 m_playerPos;

        void handleUI();
        void drawUI(AssetManager* assets);
    };
};