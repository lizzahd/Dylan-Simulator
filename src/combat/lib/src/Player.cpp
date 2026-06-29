//
// Created by xerox on 6/28/2026.
//

#include "../include/Player.h"
#include "../include/Projectile.h"

#define MOVE_SPEED 2

namespace combat {
    void Player::update() {
        // Input
        const raylib::Vector2 mousePos = GetMousePosition();
        m_angle = std::atan2f(mousePos.y - m_pos.y, mousePos.x - m_pos.x);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_entityManager->create<Projectile>(EntityType::Enemy, m_pos, m_angle, 1, 3);
        }

        if (IsKeyDown(KEY_A)) {
            m_pos.x -= MOVE_SPEED;
        }
        if (IsKeyDown(KEY_D)) {
            m_pos.x += MOVE_SPEED;
        }
        if (IsKeyDown(KEY_W)) {
            m_pos.y -= MOVE_SPEED;
        }
        if (IsKeyDown(KEY_S)) {
            m_pos.y += MOVE_SPEED;
        }

        Character::update();
    }

    void Player::draw() const {
        m_pos.DrawCircle(m_radius, GREEN);
    }
} // combat