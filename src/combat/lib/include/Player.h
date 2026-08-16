//
// Created by xerox on 6/28/2026.
//

#pragma once

#include "Character.h"

namespace combat {
    enum class PlayerCharacter {
        Dylan,
        Dorian,
        Dubi,
        Mauzer,
    };

    raylib::Color get_character_color(const PlayerCharacter &character);

    class Player : public Character {
    public:
        Player(const int id, ILocals *iLocals, const raylib::Vector2 pos)
            : Character(id, iLocals, EntityType::Player, pos, 16, 100) {
            sig_on_hurt.connect(m_id, [this] (const auto &args) {
                on_character_hurt(args);
            });
            sig_on_death.connect(m_id, [this] (const auto &args) {
                on_character_death(args);
            });
        }

        ~Player() override {
            sig_on_hurt.disconnect(m_id);
            sig_on_death.disconnect(m_id);
            Character::~Character();
        }

        void update() override;
        void draw() const override;
        void drawOverlay() const;

        void on_character_hurt(const json &args);
        void on_character_death(const json &args) const;

        int m_targetEnemy = -1;
        int m_maxWeaponTime = 200; /// Maximum weapon charge time in ticks
        int m_currentWeaponTime = 0; /// Bar moving on weapon charge wheel
        int m_damageThreshold = 30; /// Size of green area on charge wheel
        int m_damageThresholdStart = m_maxWeaponTime / 2 - m_damageThreshold / 2; /// Start of green area on charge wheel

        // Dubi combat
        float m_maxChargeUp = 20.0f;
        float m_chargeUp = 0.0f;

        std::vector<PlayerCharacter> m_characters = {PlayerCharacter::Mauzer, PlayerCharacter::Dubi};
        int m_currentCharacter = 0;
    };
} // combat