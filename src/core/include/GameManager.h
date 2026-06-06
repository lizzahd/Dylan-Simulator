//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <optional>

#include <Dialogue.h>

class GameManager {
public:
    ~GameManager() = default;
    GameManager() = default;

    void update();
    void draw() const;
    void pause();
    void unpause();
    void showDialogue(const DialogueText &dialogueText);
    void closeDialogue();

    bool m_paused = false;
    double m_time = 0.0;
    std::optional<DialogueText> m_dialogueText;
};
