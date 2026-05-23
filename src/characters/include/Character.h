//
// Created by xerox on 5/16/2026.
//
#pragma once

#include <Actor.h>
#include "Animation.h"

enum class Direction {
    Right,
    DownRight,
    Down,
    DownLeft,
    Left,
    UpLeft,
    Up,
    UpRight,
};

class Character : public Actor {
public:
    explicit Character(
        ENTITY_REQUIREMENTS,
        const EntityType entityType,
        std::vector<Animation> animationBank,
        const raylib::Vector2 pos,
        const raylib::Vector2 size)
        : Actor(ENTITY_PARAMETERS, EntityBroadType::Character, entityType, pos, size)
        , m_direction(Direction::Down)
        , m_animationBank(std::move(animationBank))
    {}

    void update() override;
    void draw() const override;
    void drawDebug() const override;
    void switchAnimation(int animationIndex);

    [[nodiscard]] float getAngleIndex() const;
    [[nodiscard]] float getAngle() const;
    [[nodiscard]] raylib::Vector2 getVector() const;

    raylib::Vector2 m_vel;
    int m_animationIndex = 0;
    Direction m_direction;

    std::vector<Animation> m_animationBank;
};