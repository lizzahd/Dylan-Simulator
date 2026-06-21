//
// Created by xerox on 5/16/2026.
//

#include <Player.h>

#include "Npc.h"

#define PLAYER_MOVE_SPEED 2.5

namespace core {
    void Player::draw() const {
        #pragma region AngleExperiments
        if (m_moving) {
            // Draw circle things
            DrawEllipse(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 64, 32, {0, 0, 0, 55});
            DrawEllipse(
                static_cast<int>(m_pos.x + (m_vel.x / PLAYER_MOVE_SPEED) * 55),
                static_cast<int>(m_pos.y + (m_vel.y / PLAYER_MOVE_SPEED) * 27),
                8,
                4,
                {255, 255, 255, 55}
            );
        }
        #pragma endregion AngleExperiments

        Character::draw();
    }

    void Player::update() {
        Character::update();

        if (const auto &transition = m_map->getTransitions(m_pos); transition.has_value()) {
            m_map->transition(transition.value(), this);
        }

        moveDirection();
    }

    void Player::moveDirection() {
        const bool up = IsKeyDown(KEY_W);
        const bool down = IsKeyDown(KEY_S);
        const bool left = IsKeyDown(KEY_A);
        const bool right = IsKeyDown(KEY_D);

        #pragma region AngleExperiments
        m_moving = false;
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            const auto [mx, my] = GetMousePosition();
            const float angle = atan2(my - m_pos.y, mx - m_pos.x);
            setAngle(angle);
            m_vel.x = cos(angle) * PLAYER_MOVE_SPEED;
            m_vel.y = sin(angle) * PLAYER_MOVE_SPEED;

            m_animationIndex = ANIM_DYLAN_WALK;
            m_animationBank[m_animationIndex].m_playing = true;
            m_moving = true;
            return;
        }
        #pragma endregion AngleExperiments

        #pragma region KeyboardInput
        // if (up && left) {
        //     m_vel.x = -PLAYER_MOVE_SPEED;
        //     m_vel.y = -PLAYER_MOVE_SPEED;
        //     m_direction = Direction::UpLeft;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (up && right) {
        //     m_vel.x = PLAYER_MOVE_SPEED;
        //     m_vel.y = -PLAYER_MOVE_SPEED;
        //     m_direction = Direction::UpRight;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (down && left) {
        //     m_vel.x = -PLAYER_MOVE_SPEED;
        //     m_vel.y = PLAYER_MOVE_SPEED;
        //     m_direction = Direction::DownLeft;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (down && right) {
        //     m_vel.x = PLAYER_MOVE_SPEED;
        //     m_vel.y = PLAYER_MOVE_SPEED;
        //     m_direction = Direction::DownRight;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (up) {
        //     m_vel.y = -PLAYER_MOVE_SPEED;
        //     m_direction = Direction::Up;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (down) {
        //     m_vel.y = PLAYER_MOVE_SPEED;
        //     m_direction = Direction::Down;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (left) {
        //     m_vel.x = -PLAYER_MOVE_SPEED;
        //     m_direction = Direction::Left;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        // if (right) {
        //     m_vel.x = PLAYER_MOVE_SPEED;
        //     m_direction = Direction::Right;
        //     m_animationIndex = ANIM_DYLAN_WALK;
        //     m_animationBank[m_animationIndex].m_playing = true;
        //     return;
        // }
        #pragma endregion KeyboardInput

        if (m_animationIndex != ANIM_DYLAN_IDLE) {
            switchAnimation(ANIM_DYLAN_IDLE);
            m_animationBank[m_animationIndex].play();
        }
    }
}