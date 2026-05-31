//
// Created by xerox on 5/22/2026.
//

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <raylib-cpp/Vector2.hpp>

#include <hot_assets/AssetManager.h>

class Animation {
public:
    ~Animation() = default;
    explicit Animation(
        const std::shared_ptr<AssetManager> &assetManager,
        std::string tex,
        const raylib::Vector2 &size,
        const raylib::Vector2 &origin,
        const int maxFrame,
        const int maxFrameTickDelay,
        const bool repeating)
        : m_assetManager(assetManager)
        , m_tex(std::move(tex))
        , m_size(size)
        , m_origin(origin)
        , m_maxFrame(maxFrame)
        , m_maxFrameTickDelay(maxFrameTickDelay)
        , m_repeating(repeating)
    {}

    void update();
    void draw(raylib::Vector2 pos, float ySrcOffset) const;
    void draw(raylib::Vector2 pos) const;
    void reset();
    void play();

    int m_currentFrame = 0;
    int m_currentFrameTickDelay = 0;
    bool m_playing = false;
    std::shared_ptr<AssetManager> m_assetManager;

    std::string m_tex;
    raylib::Vector2 m_size;
    raylib::Vector2 m_origin;

    int m_maxFrame;
    int m_maxFrameTickDelay;
    bool m_repeating;
};
