//
// Created by xerox on 5/22/2026.
//

#pragma once

#include <algorithm>
#include <raylib-cpp/Vector2.hpp>
#include <memory>
#include <raylib-cpp/Color.hpp>
#include <Actor.h>
#include <functional>

#include <hot_assets/AssetManager.h>

#define INTERACTION_DIST 150

using InteractableCallback = std::function<void(ENTITY_REQUIREMENTS)>;

class Interactable : public Actor {
public:
    explicit Interactable(
        ENTITY_REQUIREMENTS,
        std::string &tex,
        const raylib::Vector2 pos, const InteractableCallback &callback)
        : Actor(ENTITY_PARAMETERS, EntityBroadType::Interactable, EntityType::Interactable, pos, assetManager->getTex(tex).GetSize())
        , m_tex(std::move(tex))
        , m_callback(callback) {
        m_layer = 0;
    }

    void update() override;
    void draw() const override;

    void interact() const;

    [[nodiscard]] bool canInteract() const;
    [[nodiscard]] bool isHovered() const;
    [[nodiscard]] raylib::Color getTint() const;

    std::string m_tex;
    InteractableCallback m_callback;
};
