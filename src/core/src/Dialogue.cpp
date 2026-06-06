//
// Created by xerox on 6/5/2026.
//

#include "Dialogue.h"

#include <raylib-cpp.hpp>

#include "StyledText.h"

void DialogueNode::draw(raylib::Vector2 pos) const {
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
    // TODO: Recursive browsing
    m_dialogueNodes[m_dialogueNodeIndex]->m_callback(m_gameManager);
}
