//
// Created by xerox on 4/25/2026.
//

#include <Map.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include <hot_entities/EntityManager.hpp>

#include <Utils.h>
#include "GameManager.h"
#include "Player.h"

#define ROOMS_PATH "../../assets/rooms/"

namespace fs = std::filesystem;
using json = nlohmann::json;

static core::DialogueTextId dialogueIdOffset = 10000;

void Geometry::drawDebug() const {
    DrawLineStrip(m_vertices.data(), m_vertices.size(), BLUE);

    for (const auto &[pos1, pos2] : m_collisionLines) {
        pos1.DrawLine(pos2, 3, RED);
    }
}

Room::Room(const std::string& roomName, std::set<std::string> &roomsToLoad, EntityManager *entityManager, GameManager *gameManager, bool fullPath) {
    // Background Textures
    std::string roomPath;
    if (fullPath) {
        roomPath = roomName;
    } else {
        roomPath = ROOMS_PATH + roomName;
    }
    for (const auto& entry : fs::directory_iterator(roomPath)) {
        fs::path path = entry.path();
        if (path.extension() != ".png") {
            continue;
        }

        const std::string pathStr = path.stem().string();
        const auto tex = LoadTexture(path.string().c_str());
        switch (pathStr[1]) {
            case 'b':
                m_backgroundLayers.push_back(tex);
                break;
            case 'f':
                m_foregroundLayers.push_back(tex);
                break;
            case 'p': // Background Parallax
                m_backgroundParallaxLayers.push_back(tex);
                break;
            case 'P': // Foreground Parallax
                // TODO
                break;
            default:
                break;
        }
    }

    // Load geometry
    std::ifstream f(roomPath + "/map.json");
    const auto &j = json::parse(f);

    const auto &transitions = j["transitions"];
    for (const auto &transition : transitions) {
        const auto &transitionPoints = transition["points"];
        std::vector<raylib::Vector2> rTransitionPoints;
        for (const auto &p : transitionPoints) {
            rTransitionPoints.emplace_back(p["x"], p["y"]);
        }

        m_transitions.emplace(transition["name"], Geometry{rTransitionPoints});
        roomsToLoad.emplace(transition["name"]);
    }

    const auto &entrances = j["entrances"];
    for (const auto &entrance : entrances) {
        m_entrances.emplace(entrance["name"], raylib::Vector2{entrance["x"], entrance["y"]});
    }

    for (const auto &g : j["areas"]) {
        const auto &points = g["points"];
        std::vector<raylib::Vector2> rPoints;
        for (const auto &p : points) {
            rPoints.emplace_back(p["x"], p["y"]);
        }

        std::vector<Line> rCollisionLines;
        const auto &collisionLines = g["collisionLines"];
        for (const auto &line : collisionLines) {
            const raylib::Vector2 pos1(line["a"]["x"], line["a"]["y"]);
            const raylib::Vector2 pos2(line["b"]["x"], line["b"]["y"]);
            rCollisionLines.emplace_back(pos1, pos2);
        }

        m_geometries.emplace_back(rPoints, rCollisionLines);
    }

    for (const auto &interactable : j["interactables"]) {
        std::string tex = interactable["tex"];
        entityManager->create<core::Interactable>(tex, raylib::Vector2(interactable["x"], interactable["y"]), static_cast<int>(interactable["dialogueId"]) + dialogueIdOffset);
    }

    for (const auto &dialogue : j["dialogue"]) {
        // DialogueText dialogueText(gameManager, dialogue["text"]);
        // for (const auto &option : dialogue["options"]) {
        //     int nextId = option["nextId"];
        //     if (nextId != -1) {
        //         nextId += dialogueId;
        //     }
        //     dialogueText.m_dialogueNodes.emplace_back(gameManager, option["text"], nextId, [](const auto&) {}); // TODO: Callback
        // }
        //
        // if (dialogue.contains("nextId")) {
        //     int nextId = dialogue["nextId"];
        //     if (nextId != -1) {
        //         nextId += dialogueId;
        //     }
        //     dialogueText.m_nextTextId = nextId;
        // }
        //
        // if (dialogue.contains("textDelay")) {
        //     dialogueText.m_maxDelay = dialogue["textDelay"];
        // }
        //
        // gameManager->m_dialogueTextMap.emplace(static_cast<int>(dialogue["id"]) + dialogueId, dialogueText);
        gameManager->m_dialogueTextMap.emplace(static_cast<int>(dialogue["id"]) + dialogueIdOffset, core::DialogueText::fromJson(gameManager, dialogue, dialogueIdOffset));
    }
}

void Room::drawBackgroundLayers() const {
    for (const auto &layer : m_backgroundLayers) {
        layer.Draw();
    }
}

void Room::drawForegroundLayers() const {
    for (const auto &layer : m_foregroundLayers) {
        layer.Draw();
    }
}

void Room::drawDebug() const {
    for (const auto &geometry : m_geometries) {
        geometry.drawDebug();
    }

    for (const auto hitbox : m_hitboxes) {
        hitbox.DrawLines(RED);
    }
}

std::optional<std::string> Room::getTransitions(const raylib::Vector2 pos) const {
    for (const auto &[roomName, t] : m_transitions) {
        if (CheckCollisionPointPoly(pos, t.m_vertices.data(), t.m_vertices.size())) {
            return roomName;
        }
    }

    return std::nullopt;
}

void Map::drawBackgroundLayers() const {
    m_rooms.at(m_currentRoom)->drawBackgroundLayers();
}

void Map::drawForegroundLayers() const {
    m_rooms.at(m_currentRoom)->drawForegroundLayers();
}

void Map::drawDebug() const {
    m_rooms.at(m_currentRoom)->drawDebug();
}

void Map::transition(const std::string &room, core::Player *player) {
    player->m_vel = 0;
    player->m_direction = core::Direction::Down;
    player->m_pos = m_rooms.at(room)->m_entrances[m_currentRoom];
    m_currentRoom = room;
}

void Map::load(const std::string &startRoomName, EntityManager *entityManager, GameManager *gameManager, const bool fullPath) {
    std::set roomsToLoad{startRoomName};
    for (const auto &roomName : roomsToLoad) {
        m_rooms.emplace(roomName, std::make_shared<Room>(roomName, roomsToLoad, entityManager, gameManager, fullPath));
        dialogueIdOffset += 1000; // Buffer between rooms
    }
    m_currentRoom = startRoomName;
}

std::vector<Geometry> &Map::getGeometries() const {
    return m_rooms.at(m_currentRoom)->m_geometries;
}

std::optional<std::string> Map::getTransitions(const raylib::Vector2 pos) const {
    return m_rooms.at(m_currentRoom)->getTransitions(pos);
}
