//
// Created by xerox on 6/5/2026.
//

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>

#define TEXT_MAX_INITIAL_DELAY 5

#define DIALOGUE_TEXT_ID_CLOSE (-1)

class GameManager;

namespace raylib {
    class Vector2;
}

namespace core {
    using DialogueOptionCallback = std::function<void(GameManager *)>;
    using DialogueTextId = int;

    class DialogueNode {
    public:
        ~DialogueNode() = default;
        explicit DialogueNode(GameManager *gameManager, std::string text, const DialogueTextId nextTextId, DialogueOptionCallback callback)
            : m_nextTextId(nextTextId)
            , m_text(std::move(text))
            , m_gameManager(gameManager)
            , m_callback(std::move(callback))
        {}

        void draw(raylib::Vector2 pos) const;

        DialogueTextId m_nextTextId;
        std::string m_text;
        GameManager *m_gameManager;
        DialogueOptionCallback m_callback;
    };

    class DialogueText {
    public:
        ~DialogueText() = default;
        explicit DialogueText(GameManager *gameManager, std::string text)
            : m_text(std::move(text))
            , m_gameManager(gameManager)
        {}
        static DialogueText fromJson(GameManager *gameManager, const nlohmann::json &dialogue, DialogueTextId offset = 0);

        void update();
        void draw(raylib::Vector2 pos) const;
        [[nodiscard]] bool isDone() const;
        [[nodiscard]] bool isInitialized() const {
            return m_initialDelay == 0;
        }
        void changeIndex(int change);
        void select();
        void reset();

        std::string m_text;
        int m_maxDelay = 1;
        int m_currentDelay = 0;
        int m_currentLength = 1;
        int m_initialDelay = 5;

        std::vector<DialogueNode> m_dialogueNodes;
        int m_dialogueNodeIndex = 0;
        std::optional<DialogueTextId> m_nextTextId;

        GameManager *m_gameManager;
    };
}