//
// Created by xerox on 5/16/2026.
//

#pragma once

#include <Character.h>

enum PlayerAnimState {
    ANIM_DYLAN_IDLE,
    ANIM_DYLAN_WALK,
};

class Player : public Character {
public:
    explicit Player(
        ENTITY_REQUIREMENTS,
        const raylib::Vector2 pos)
        : Character(
            ENTITY_PARAMETERS,
            EntityType::Player,
            {
                Animation(assetManager, "dylan_idle", raylib::Vector2(140, 140), {70, 115}, 5, 9, true),
                Animation(assetManager, "dylan_walk", raylib::Vector2(140, 140), {70, 115}, 3, 9, true),
            },
            pos,
            {140, 140})
    {}

    void update() override;
    void moveDirection();

    [[nodiscard]] raylib::Rectangle getRect() const override {
        const auto &anim = m_animationBank[m_animationIndex];
        return raylib::Rectangle{m_pos.x + 48 - anim.m_origin.x, m_pos.y + 9 - anim.m_origin.y, 48, 112};
    }
};