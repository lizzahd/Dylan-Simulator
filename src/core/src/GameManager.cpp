//
// Created by xerox on 6/5/2026.
//

#include <GameManager.h>

#include <raylib.h>
#include <raylib-cpp/Rectangle.hpp>

#include <StyledText.h>
#include "Utils.h"

void GameManager::update() {
    if (m_dialogueText.has_value()) {
        m_dialogueText->update();
        if (isInteractKeyPressed()) {
            if (m_dialogueText->isInitialized() && m_dialogueText->isDone() && m_dialogueText->m_dialogueNodes.empty()) {
                unpause();
                m_dialogueText.reset();
                return;
            } else if (m_dialogueText->m_initialDelay <= TEXT_MAX_INITIAL_DELAY && m_dialogueText->isInitialized()) {
                m_dialogueText->m_currentLength = m_dialogueText->m_text.length();
            }
        }

        // Dialogue option navigation
        if (m_dialogueText->isDone() && !m_dialogueText->m_dialogueNodes.empty()) {
            if (IsKeyPressed(KEY_UP)) {
                m_dialogueText->changeIndex(-1);
            }
            if (IsKeyPressed(KEY_DOWN)) {
                m_dialogueText->changeIndex(1);
            }
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                m_dialogueText->select();
            }
        }
    }

    m_time += 1;
}

void GameManager::draw() const {
    if (m_dialogueText.has_value()) {
        // Get textbox width
        const float width = GetScreenWidth();
        const float height = GetScreenHeight();
        const raylib::Rectangle textBox{
            10,
            height - height / 3 - 10,
            width - 20,
            height / 3
        };
        DrawRectangleRounded(textBox, 0.15, 0, BLACK);
        DrawRectangleRoundedLines(textBox, 0.15, 0, WHITE);

        // Draw dialogue text
        m_dialogueText->draw({textBox.x + 10, textBox.y + 10});

        if (m_dialogueText->isDone() && !m_dialogueText->m_dialogueNodes.empty()) {
            float y = textBox.y + textBox.height / 2;
            DrawText(">", 20, y + m_dialogueText->m_dialogueNodeIndex * 24, 20, WHITE);
            for (const auto &node : m_dialogueText->m_dialogueNodes) {
                node->draw({30, y});
                y += 24;
            }
        }
    }
}

void GameManager::pause() {
    m_paused = true;
}

void GameManager::unpause() {
    m_paused = false;
}

void GameManager::showDialogue(const DialogueText &dialogueText) {
    pause();
    m_dialogueText = dialogueText;
}

void GameManager::closeDialogue() {
    m_dialogueText.reset();
    unpause();
}
