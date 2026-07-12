//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <Character.h>

namespace core {
    class Npc : public Character {
    public:
        explicit Npc(
            ENTITY_REQUIREMENTS,
            const std::vector<CharacterAnimation> &animationBank,
            const raylib::Vector2 pos,
            const raylib::Vector2 size)
            : Character(
                ENTITY_PARAMETERS, EntityType::Npc, animationBank, pos, size) {
            m_animationBank[m_animationIndex].m_playing = true;
        }

        void update() override;
    };

    class Dubi : public Npc {
    public:
        Dubi(ENTITY_REQUIREMENTS, const raylib::Vector2 pos)
        : Npc(ENTITY_PARAMETERS, {
                CharacterAnimation(assetManager, "dubi_idle", raylib::Vector2(140, 140), {70, 115}, 5, 9, true),
                CharacterAnimation(assetManager, "dubi_walk", raylib::Vector2(140, 140), {70, 115}, 5, 9, true),
            }, pos, {140, 140})
        {}

        [[nodiscard]] raylib::Rectangle getRect() const override {
            const auto &anim = m_animationBank[m_animationIndex];
            return raylib::Rectangle{m_pos.x + 48 - anim.m_origin.x, m_pos.y + 41 - anim.m_origin.y, 48, 80};
        }

        [[nodiscard]] DialogueTextId getDialogue() const override {
            return 100;
        }
    };

    class Dorian : public Npc {
    public:
        Dorian(ENTITY_REQUIREMENTS, const raylib::Vector2 pos)
        : Npc(ENTITY_PARAMETERS, {
                CharacterAnimation(assetManager, "dorian_idle", raylib::Vector2(140, 140), {70, 115}, 1, 9, true),
                CharacterAnimation(assetManager, "dorian_walk", raylib::Vector2(140, 140), {70, 115}, 1, 9, true),
            }, pos, {140, 140})
        {}

        [[nodiscard]] raylib::Rectangle getRect() const override {
            const auto &anim = m_animationBank[m_animationIndex];
            return raylib::Rectangle{m_pos.x + 48 - anim.m_origin.x, m_pos.y + 21 - anim.m_origin.y, 48, 100};
        }

        [[nodiscard]] DialogueTextId getDialogue() const override {
            return 200;
        }
    };
}