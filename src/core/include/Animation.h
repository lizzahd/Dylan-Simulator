//
// Created by xerox on 5/22/2026.
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <raylib-cpp/Vector2.hpp>

#include <hot_assets/AssetManager.h>

namespace core {
    class Animation {
    public:
        ~Animation() = default;
        explicit Animation(
            AssetManager *assetManager,
            const int maxFrame,
            const int maxFrameTickDelay,
            const bool repeating)
            : m_assetManager(assetManager)
            , m_maxFrame(maxFrame)
            , m_maxFrameTickDelay(maxFrameTickDelay)
            , m_repeating(repeating)
        {}

        void update();
        void reset();
        void play();

        int m_currentFrame = 0;
        int m_currentFrameTickDelay = 0;
        bool m_playing = false;
        AssetManager *m_assetManager;

        int m_maxFrame;
        int m_maxFrameTickDelay;
        bool m_repeating;
    };
}