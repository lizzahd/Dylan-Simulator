//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#define TEXT_MAX_INITIAL_DELAY 5

namespace raylib {
    class Vector2;
}

class GameManager;

using DialogOptionCallback = std::function<void(const std::shared_ptr<GameManager>&)>;

class DialogueNode {
public:
    ~DialogueNode() = default;
    DialogueNode(const std::shared_ptr<GameManager> &gameManager, std::string text, const std::shared_ptr<DialogueNode> &parent, const DialogOptionCallback &callback)
        : m_text(std::move(text))
        , m_parent(parent)
        , m_gameManager(gameManager)
        , m_callback(callback)
    {}
    explicit DialogueNode(const std::shared_ptr<GameManager> &gameManager, std::string text, const DialogOptionCallback &callback)
        : m_text(std::move(text))
        , m_gameManager(gameManager)
        , m_callback(callback)
    {}

    void draw(raylib::Vector2 pos) const;

    std::string m_text;
    std::vector<std::shared_ptr<DialogueNode>> m_children;
    std::weak_ptr<DialogueNode> m_parent;
    std::shared_ptr<GameManager> m_gameManager;
    DialogOptionCallback m_callback;
};

class DialogueText {
public:
    ~DialogueText() = default;
    DialogueText(const std::shared_ptr<GameManager> &gameManager, std::string text, const int maxDelay)
        : m_text(std::move(text))
        , m_maxDelay(maxDelay)
        , m_gameManager(gameManager)
    {}

    void update();
    void draw(raylib::Vector2 pos) const;
    [[nodiscard]] bool isDone() const;
    [[nodiscard]] bool isInitialized() const {
        return m_initialDelay == 0;
    }
    void changeIndex(int change);
    void select();

    std::string m_text;
    int m_maxDelay;
    int m_currentDelay = 0;
    int m_currentLength = 1;
    int m_initialDelay = 5;

    std::vector<std::shared_ptr<DialogueNode>> m_dialogueNodes;
    int m_dialogueNodeIndex = 0;
    std::vector<int> m_path; // Path through recursive nodes

    std::shared_ptr<GameManager> m_gameManager;
};