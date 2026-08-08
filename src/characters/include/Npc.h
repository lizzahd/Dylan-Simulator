//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <Character.h>

namespace core {
    class Npc : public Character {
    public:
        explicit Npc(
            const int id,
            ILocals *iLocals,
            const std::vector<CharacterAnimation> &animationBank,
            const raylib::Vector2 pos,
            const raylib::Vector2 size)
            : Character(id, iLocals, EntityType::Npc, animationBank, pos, size)
        {}

        void update() override;
    };

    class Dubi : public Npc {
    public:
        Dubi(const int id, ILocals *iLocals, const raylib::Vector2 pos)
        : Npc(id, iLocals, {}, pos, {140, 140}) {
            m_animationBank = {
                CharacterAnimation(m_assetManager, "dubi_idle", raylib::Vector2(140, 140), {70, 115}, 5, 9, true, true),
                CharacterAnimation(m_assetManager, "dubi_walk", raylib::Vector2(140, 140), {70, 115}, 5, 9, true),
            };
        }

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
        Dorian(const int id, ILocals *iLocals, const raylib::Vector2 pos)
        : Npc(id, iLocals, {}, pos, {140, 140}) {
            m_animationBank = {
                CharacterAnimation(m_assetManager, "dorian_idle", raylib::Vector2(140, 140), {70, 115}, 1, 9, true, true),
                CharacterAnimation(m_assetManager, "dorian_walk", raylib::Vector2(140, 140), {70, 115}, 1, 9, true),
            };
        }

        [[nodiscard]] raylib::Rectangle getRect() const override {
            const auto &anim = m_animationBank[m_animationIndex];
            return raylib::Rectangle{m_pos.x + 48 - anim.m_origin.x, m_pos.y + 21 - anim.m_origin.y, 48, 100};
        }

        [[nodiscard]] DialogueTextId getDialogue() const override {
            return 200;
        }
    };
}