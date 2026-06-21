//
// Created by xerox on 4/25/2026.
//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <set>
#include <raylib-cpp/Rectangle.hpp>
#include <raylib-cpp/Texture.hpp>

struct Line;
class EntityManager;

namespace core {
    class Player;
}

class GameManager;

struct Geometry {
    std::vector<raylib::Vector2> m_vertices;
    std::vector<Line> m_collisionLines;

    void drawDebug() const;
};

/// Not in namespace core because map is passed as a parameter into entityManager
struct Room {
    ~Room() = default;
    Room() = default;
    explicit Room(const std::string &roomName, std::set<std::string> &roomsToLoad, EntityManager *entityManager, GameManager *gameManager);

    void drawBackgroundLayers() const;
    void drawForegroundLayers() const;
    void drawDebug() const;
    [[nodiscard]] std::optional<std::string> getTransitions(raylib::Vector2 pos) const;

    // The key is the id of the previous area
    std::unordered_map<std::string, raylib::Vector2> m_entrances;

    // The key is the id of the area it takes you to
    std::unordered_map<std::string, Geometry> m_transitions;

    std::vector<raylib::Rectangle> m_hitboxes;
    std::vector<raylib::Texture2D> m_backgroundLayers;
    std::vector<raylib::Texture2D> m_foregroundLayers;
    std::vector<raylib::Texture2D> m_backgroundParallaxLayers;

    // Physics and shite
    std::vector<Geometry> m_geometries;
};

/// Not in namespace core because it's passed as a parameter into entityManager
class Map {
public:
    ~Map() = default;
    Map() = default;

    void drawBackgroundLayers() const;
    void drawForegroundLayers() const;
    void drawDebug() const;
    void transition(const std::string &room, core::Player *player);
    void load(const std::string &startRoomName, EntityManager *entityManager, GameManager *gameManager);
    [[nodiscard]] std::vector<Geometry> &getGeometries() const;
    [[nodiscard]] std::optional<std::string> getTransitions(raylib::Vector2 pos) const;

    // Area transitioning
    std::unordered_map<std::string, std::shared_ptr<Room>> m_rooms;
    std::string m_currentRoom;
};
