//
// Created by xerox on 5/22/2026.
//

#pragma once

#include <algorithm>
#include <vector>
#include <raylib-cpp/Vector2.hpp>
#include <memory>
#include <raylib-cpp/Color.hpp>
#include <Actor.h>

#include "AssetManager.h"

class AssetManager;

class Interactable : public Actor {
public:
    explicit Interactable(
        ENTITY_REQUIREMENTS,
        std::string &tex,
        const raylib::Vector2 pos,
        const std::vector<raylib::Vector2> &points)
        : Actor(ENTITY_PARAMETERS, EntityBroadType::Interactable, EntityType::Interactable, pos, assetManager->getTex(tex).GetSize())
        , m_tex(std::move(tex)) {
        m_layer = 0;

        // Offset points for easier setup
        for (const auto &point : points) {
            m_points.push_back(point + pos);
        }
    }

    void update() override;
    void draw() const override;
    void drawDebug() const override;

    [[nodiscard]] bool canInteract() const;
    [[nodiscard]] bool isHovered() const;
    [[nodiscard]] raylib::Color getTint() const;

    std::string m_tex;
    std::vector<raylib::Vector2> m_points;
};
