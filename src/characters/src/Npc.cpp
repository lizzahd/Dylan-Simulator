//
// Created by xerox on 6/5/2026.
//

#include <Npc.h>

#include "Player.h"

void Npc::update() {
    Character::update();

    m_entityManager->breakableExecByType<Character>([&](int, const auto character) {
        if (character->getType() != EntityType::Player) {
            return false;
        }

        // Get distance
        // const float dist = character->m_pos.Distance(m_pos);
        // if (dist < NPC_LOOKING_DISTANCE) {
            // Look at player
            const float angle = atan2(character->m_pos.y - m_pos.y, character->m_pos.x - m_pos.x);
            setAngle(angle);
        // }

        return true;
    });
}
