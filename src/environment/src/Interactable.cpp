//
// Created by xerox on 5/22/2026.
//


#include <algorithm>

#include "hot_assets/AssetManager.h"

#include "Interactable.h"
#include "Character.h"
#include <Actor.h>

#define INTERACTION_DIST 150

void Interactable::update() {

}

void Interactable::draw() const {
    m_assetManager->getTex(m_tex).Draw(
        raylib::Rectangle{0, 0, m_size.x, m_size.y},
        raylib::Rectangle{m_pos, m_size},
        raylib::Vector2{},
        0,
        getTint()
    );
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
