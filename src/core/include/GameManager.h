//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <Dialogue.h>

/// Not in namespace core because it's passed as a parameter into entityManager
class GameManager : public std::enable_shared_from_this<GameManager> {
public:
    ~GameManager() = default;
    GameManager() = default;

    void init();
    void update();
    void draw() const;
    void pause();
    void unpause();
    void showDialogue(core::DialogueTextId dialogueTextId);
    void closeDialogue();

    bool m_paused = false;
    double m_time = 0.0;
    std::unordered_map<core::DialogueTextId, core::DialogueText> m_dialogueTextMap;
    core::DialogueTextId m_dialogueTextId = DIALOGUE_TEXT_ID_CLOSE;
};
