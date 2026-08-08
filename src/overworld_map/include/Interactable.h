//
// Created by xerox on 5/22/2026.
//

#pragma once

#include <hot_assets/AssetManager.h>

#include <algorithm>
#include <raylib-cpp/Vector2.hpp>
#include <raylib-cpp/Color.hpp>
#include <functional>

#include <Actor.h>
#include "Dialogue.h"

namespace core {
    #define INTERACTION_DIST 150

    using InteractableCallback = std::function<void(int id, ILocals *iLocals)>;

    class Interactable : public Actor {
    public:
        explicit Interactable(
            const int id,
            ILocals *iLocals,
            std::string &tex,
            const raylib::Vector2 pos, const DialogueTextId dialogueTextId)
            : Actor(id, iLocals, EntityBroadType::Interactable, EntityType::Interactable, pos, {})
            , m_tex(std::move(tex))
            , m_dialogueTextId(dialogueTextId) {
            m_layer = 0;
            m_size = m_assetManager->getTex(m_tex).GetSize();
        }

        void update() override;
        void draw() const override;

        void interact() const;

        [[nodiscard]] bool canInteract() const;
        [[nodiscard]] bool isHovered() const;
        [[nodiscard]] raylib::Color getTint() const;

        std::string m_tex;
        DialogueTextId m_dialogueTextId;
    };
}