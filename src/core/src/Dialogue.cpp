//
// Created by xerox on 6/5/2026.
//

#include "Dialogue.h"

#include <raylib-cpp.hpp>

#include "GameManager.h"
#include "StyledText.h"

void DialogueNode::draw(const raylib::Vector2 pos) const {
    drawStyledText(m_gameManager, GetFontDefault(), m_text, pos, 20, 2, WHITE, 0);
}

void DialogueText::update() {
    if (!isInitialized()) {
        m_initialDelay--;
        return;
    }

    if (isDone()) {
        return;
    }

    if (m_currentDelay++ >= m_maxDelay && m_currentLength < m_text.length()) {
        m_currentDelay = 0;
        m_currentLength++;
    }
}

void DialogueText::draw(const raylib::Vector2 pos) const {
    if (!isInitialized()) {
        return;
    }

    drawStyledText(m_gameManager, GetFontDefault(), m_text, pos, 30, 2, WHITE, m_currentLength);
}

bool DialogueText::isDone() const {
    return m_currentLength >= m_text.length();
}

void DialogueText::changeIndex(const int change) {
    m_dialogueNodeIndex += change;
    if (m_dialogueNodeIndex >= m_dialogueNodes.size()) {
        m_dialogueNodeIndex = 0;
    } else if (m_dialogueNodeIndex < 0) {
        m_dialogueNodeIndex = m_dialogueNodes.size() - 1;
    }
}

void DialogueText::select() {
    if (!isInitialized()) {
        return;
    }

    m_dialogueNodes[m_dialogueNodeIndex].m_callback(m_gameManager);
    m_gameManager->showDialogue(m_dialogueNodes[m_dialogueNodeIndex].m_nextTextId);
    reset();
}

void DialogueText::reset() {
    m_currentDelay = 0;
    m_currentLength = 1;
    m_initialDelay = 5;
    m_dialogueNodeIndex = 0;
}
