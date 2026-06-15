//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <Dialogue.h>

class GameManager {
public:
    ~GameManager() = default;
    GameManager() = default;

    void update();
    void draw() const;
    void pause();
    void unpause();
    void showDialogue(DialogueTextId dialogueTextId);
    void closeDialogue();

    bool m_paused = false;
    double m_time = 0.0;
    std::unordered_map<DialogueTextId, DialogueText> m_dialogueTextMap;
    DialogueTextId m_dialogueTextId = DIALOGUE_TEXT_ID_CLOSE;
};
