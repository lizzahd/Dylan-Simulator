//
// Created by xerox on 5/16/2026.
//
#pragma once

#include <Actor.h>
#include "Animation.h"
#include "CharacterAnimation.h"
#include "Interactable.h"

#define FALLING_VELOCITY 3

namespace core {
    enum CharacterAnimState {
        ANIM_GENERIC_IDLE,
        ANIM_GENERIC_WALK,
    };

    enum class Direction {
        Right,
        Down,
        Left,
        Up,
    };

    class Character : public Actor {
    public:
        explicit Character(
            ENTITY_REQUIREMENTS,
            const EntityType entityType,
            std::vector<CharacterAnimation> animationBank,
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

        void setAngle(float angle);
        [[nodiscard]] float getAngleIndex() const;
        [[nodiscard]] float getAngle() const;
        [[nodiscard]] raylib::Vector2 getVector() const;

        // Interactability
        void interact() const;
        [[nodiscard]] bool canInteract() const;
        [[nodiscard]] bool isHovered() const;
        [[nodiscard]] virtual DialogueTextId getDialogue() const = 0;

        /// Takes in a leader id, returns follower index
        void follow(int id);
        void stopFollow();

        raylib::Vector2 m_vel;
        int m_animationIndex = 0;
        Direction m_direction;
        float m_fallingVel = 0;

        std::vector<CharacterAnimation> m_animationBank;
        InteractableCallback m_callback;

        int m_leader = -1;
        int m_followerIndex = 0;
        std::set<int> m_followers;
    };
}