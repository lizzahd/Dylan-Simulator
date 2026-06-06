//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <Character.h>

#define NPC_LOOKING_DISTANCE 100

class Npc : public Character {
public:
    explicit Npc(
        ENTITY_REQUIREMENTS,
        const EntityType entityType,
        const std::vector<Animation> &animationBank,
        const raylib::Vector2 pos,
        const raylib::Vector2 size)
        : Character(
            ENTITY_PARAMETERS, entityType, animationBank, pos, size) {
        m_animationBank[m_animationIndex].m_playing = true;
    }

    void update() override;
};