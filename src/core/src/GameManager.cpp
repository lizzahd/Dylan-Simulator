//
// Created by xerox on 6/5/2026.
//

#include <GameManager.h>

#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raylib-cpp/Rectangle.hpp>

#include <StyledText.h>
#include <nlohmann/json.hpp>

#include "Utils.h"

using json = nlohmann::json;

void GameManager::init() {
    // Load base.json
    std::ifstream f("../../assets/base.json");
    const auto &j = json::parse(f);
    for (const auto &dialogue : j["dialogue"]) {
        m_dialogueTextMap.emplace(dialogue[0]["id"], DialogueText::fromJson(this, dialogue[0]));
    }
}

void GameManager::update() {
    if (m_dialogueTextId != DIALOGUE_TEXT_ID_CLOSE) {
        DialogueText &dialogueText = m_dialogueTextMap.at(m_dialogueTextId);
        dialogueText.update();

        // Dialogue option navigation
        if (dialogueText.isDone() && !dialogueText.m_dialogueNodes.empty() && dialogueText.isInitialized()) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                dialogueText.changeIndex(-1);
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                dialogueText.changeIndex(1);
            }
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                dialogueText.select();
                return;
            }
        }

        if (isInteractKeyPressed()) {
            if (dialogueText.isInitialized() && dialogueText.isDone() && dialogueText.m_dialogueNodes.empty()) {
                if (dialogueText.m_nextTextId.has_value()) {
                    showDialogue(dialogueText.m_nextTextId.value());
                } else {
                    closeDialogue();
                }
                return;
            } else if (dialogueText.m_initialDelay <= TEXT_MAX_INITIAL_DELAY && dialogueText.isInitialized()) {
                dialogueText.m_currentLength = dialogueText.m_text.length();
            }
        }
    }

    m_time += 1;
}

void GameManager::draw() const {
    if (m_dialogueTextId != DIALOGUE_TEXT_ID_CLOSE) {
        const DialogueText &dialogueText = m_dialogueTextMap.at(m_dialogueTextId);
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
        dialogueText.draw({textBox.x + 10, textBox.y + 10});

        if (dialogueText.isDone() && !dialogueText.m_dialogueNodes.empty()) {
            float y = textBox.y + textBox.height / 2;
            DrawText(">", 20, y + dialogueText.m_dialogueNodeIndex * 24, 20, WHITE);
            for (const auto &node : dialogueText.m_dialogueNodes) {
                node.draw({30, y});
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

void GameManager::showDialogue(const DialogueTextId dialogueTextId) {
    if (dialogueTextId == DIALOGUE_TEXT_ID_CLOSE) {
        closeDialogue();
        return;
    }

    pause();
    m_dialogueTextId = dialogueTextId;
    m_dialogueTextMap.at(m_dialogueTextId).reset();
}

void GameManager::closeDialogue() {
    if (m_dialogueTextId != DIALOGUE_TEXT_ID_CLOSE) {
        m_dialogueTextMap.at(m_dialogueTextId).reset();
    }

    m_dialogueTextId = DIALOGUE_TEXT_ID_CLOSE;
    unpause();
}
