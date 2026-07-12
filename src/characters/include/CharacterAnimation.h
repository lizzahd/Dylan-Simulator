//
// Created by xerox on 7/11/2026.
//

#pragma once
#include "Animation.h"

namespace core {
    enum ANIMATION_DRAW_FLAGS {
        ANIMATION_DRAW_FLAG_NONE        = 0,
        ANIMATION_DRAW_FLAG_SHADOW      = 1 << 0,
        ANIMATION_DRAW_FLAG_OUTLINE     = 1 << 1,
    };

    struct AnimationEffectParams {
        float outlineColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float outlineThickness = 1.0f;
    };

    class CharacterAnimation : public Animation {
    public:
        CharacterAnimation(
            AssetManager *assetManager,
            std::string tex,
            const raylib::Vector2 &size,
            const raylib::Vector2 &origin,
            const int maxFrame,
            const int maxFrameTickDelay,
            const bool repeating)
            : Animation(maxFrame, maxFrameTickDelay, repeating)
            , m_tex(std::move(tex))
            , m_size(size)
            , m_origin(origin)
            , m_assetManager(assetManager)
        {}

        void draw(raylib::Vector2 pos, float ySrcOffset, int flags = ANIMATION_DRAW_FLAG_NONE, const AnimationEffectParams &effectParams = {}) const;
        void draw(raylib::Vector2 pos, int flags = ANIMATION_DRAW_FLAG_NONE, const AnimationEffectParams &effectParams = {}) const;
        void drawOutline(raylib::Vector2 pos, float ySrcOffset, Color color) const;
        [[nodiscard]] raylib::Rectangle getSourceRect(float ySrcOffset) const;

        std::string m_tex;
        raylib::Vector2 m_size;
        raylib::Vector2 m_origin;

        AssetManager *m_assetManager;
    };
}
