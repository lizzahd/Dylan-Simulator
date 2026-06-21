//
// Created by xerox on 5/22/2026.
//


#include <algorithm>

#include "hot_assets/AssetManager.h"
#include <hot_entities/EntityManager.hpp>

#include "Interactable.h"
#include "Character.h"
#include <Actor.h>

#include "GameManager.h"

namespace core {
    void Interactable::update() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && canInteract() && isHovered()) {
            interact();
        }
    }

    void Interactable::draw() const {
        const bool hasOutline = isHovered();
        if (hasOutline) {
            float outlineColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            if (!canInteract()) {
                outlineColor[0] = 0.5f;
                outlineColor[1] = 0.5f;
                outlineColor[2] = 0.5f;
            }
            constexpr float outlineSize = 1.0f;
            const float textureSize[2] = { m_size.x, m_size.y };

            auto &shader = m_assetManager->getShader("outline");
            const int outlineSizeLoc = shader.GetLocation("outlineSize");
            const int outlineColorLoc = shader.GetLocation("outlineColor");
            const int textureSizeLoc = shader.GetLocation("textureSize");
            shader.SetValue(outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
            shader.SetValue(outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);
            shader.SetValue(textureSizeLoc, textureSize, SHADER_UNIFORM_VEC2);

            BeginShaderMode(shader);
        }

        m_assetManager->getTex(m_tex).Draw(
            raylib::Rectangle{0, 0, m_size.x, m_size.y},
            raylib::Rectangle{m_pos, m_size},
            raylib::Vector2{},
            0,
            getTint()
        );

        if (hasOutline) {
            EndShaderMode();
        }
    }

    void Interactable::interact() const {
        m_gameManager->showDialogue(m_dialogueTextId);
    }

    bool Interactable::canInteract() const {
        // TODO: Un-Hardcode
        return m_entityManager->breakableExecByType<Character>([&](const int, const auto other) {
            if (other->getType() != EntityType::Player) {
                return false;
            }

            if (m_pos.Distance(other->m_pos) < INTERACTION_DIST) {
                return true;
            }

            return false;
        });
    }

    bool Interactable::isHovered() const {
        // Check if mouse position on pixel
        // This might be expensive
        const auto [mx, my] = GetMousePosition();
        const int x = static_cast<int>(mx - m_pos.x);
        const int y = static_cast<int>(my - m_pos.y);
        if (x < 0 || x >= m_size.x || y < 0 || y >= m_size.y) {
            return false;
        }
        const raylib::Color color = GetImageColor(m_assetManager->getTex(m_tex), x, y);
        return color.a > 0;
    }

    raylib::Color Interactable::getTint() const {
        if (!isHovered()) {
            return WHITE;
        }

        if (canInteract()) {
            return LIME;
        }

        return RED;
    }
}