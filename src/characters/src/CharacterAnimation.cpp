//
// Created by xerox on 7/11/2026.
//

#include <CharacterAnimation.h>

namespace core {
    void CharacterAnimation::draw(const raylib::Vector2 pos, const float ySrcOffset, const int flags, const AnimationEffectParams &effectParams) const {
        const raylib::Rectangle src{static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y};
        const raylib::Rectangle dst{{std::round(pos.x), std::round(pos.y)}, {m_size.x, m_size.y}};
        const auto &tex = m_assetManager->getTex(m_tex);

        if (flags & ANIMATION_DRAW_FLAG_OUTLINE) {
            const float textureSize[2] = { static_cast<float>(tex.GetWidth()), static_cast<float>(tex.GetHeight()) };

            auto &shader = m_assetManager->getShader("outline");
            const int outlineSizeLoc = shader.GetLocation("outlineSize");
            const int outlineColorLoc = shader.GetLocation("outlineColor");
            const int textureSizeLoc = shader.GetLocation("textureSize");
            shader.SetValue(outlineSizeLoc, &effectParams.outlineThickness, SHADER_UNIFORM_FLOAT);
            shader.SetValue(outlineColorLoc, effectParams.outlineColor, SHADER_UNIFORM_VEC4);
            shader.SetValue(textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

            BeginShaderMode(shader);
        }

        // const auto &shader = m_assetManager->getShader("pixelate");
        // BeginShaderMode(shader);

        tex.Draw(
            src,
            dst,
            m_origin
        );

        // EndShaderMode();

        if (flags & ANIMATION_DRAW_FLAG_OUTLINE) {
            EndShaderMode();
        }
    }

    void CharacterAnimation::draw(const raylib::Vector2 pos, const int flags, const AnimationEffectParams &effectParams) const {
        draw(pos, 0, flags, effectParams);
    }

    void CharacterAnimation::drawOutline(const raylib::Vector2 pos, const float ySrcOffset, const Color color) const {
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

    raylib::Rectangle CharacterAnimation::getSourceRect(const float ySrcOffset) const {
        return {static_cast<float>(m_currentFrame) * m_size.x, ySrcOffset * m_size.y, m_size.x, m_size.y};
    }
}