//
// Created by xerox on 5/22/2026.
//


#include <algorithm>

#include "Interactable.h"
#include "AssetManager.h"
#include <EntityManager.h>

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

void Interactable::drawDebug() const {
    DrawTriangleFan(m_points.data(), m_points.size(), LIME);
}

bool Interactable::canInteract() const {
    // TODO: Un-Hardcode
    return m_entityManager->breakableExecByType<Character>([&](const int, const auto other) {
        if (other->getType() != EntityType::Player) {
            return false;
        }

        if (m_pos.Distance(other->m_pos) < 150) {
            return true;
        }

        return false;
    });
}

bool Interactable::isHovered() const {
    return CheckCollisionPointPoly(GetMousePosition(), m_points.data(), m_points.size());
}

raylib::Color Interactable::getTint() const {
    if (!isHovered()) {
        return WHITE;
    }

    if (canInteract()) {
        return LIME;
    }

    return GRAY;
}
