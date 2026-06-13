//
// Created by xerox on 5/16/2026.
//

#include <Player.h>

#define PLAYER_MOVE_SPEED 2.5

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

    if (up && left) {
        m_vel.x = -PLAYER_MOVE_SPEED;
        m_vel.y = -PLAYER_MOVE_SPEED;
        m_direction = Direction::UpLeft;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (up && right) {
        m_vel.x = PLAYER_MOVE_SPEED;
        m_vel.y = -PLAYER_MOVE_SPEED;
        m_direction = Direction::UpRight;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (down && left) {
        m_vel.x = -PLAYER_MOVE_SPEED;
        m_vel.y = PLAYER_MOVE_SPEED;
        m_direction = Direction::DownLeft;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (down && right) {
        m_vel.x = PLAYER_MOVE_SPEED;
        m_vel.y = PLAYER_MOVE_SPEED;
        m_direction = Direction::DownRight;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (up) {
        m_vel.y = -PLAYER_MOVE_SPEED;
        m_direction = Direction::Up;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (down) {
        m_vel.y = PLAYER_MOVE_SPEED;
        m_direction = Direction::Down;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (left) {
        m_vel.x = -PLAYER_MOVE_SPEED;
        m_direction = Direction::Left;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }
    if (right) {
        m_vel.x = PLAYER_MOVE_SPEED;
        m_direction = Direction::Right;
        m_animationIndex = ANIM_DYLAN_WALK;
        m_animationBank[m_animationIndex].m_playing = true;
        return;
    }

    if (m_animationIndex != ANIM_DYLAN_IDLE) {
        switchAnimation(ANIM_DYLAN_IDLE);
        m_animationBank[m_animationIndex].play();
    }
}