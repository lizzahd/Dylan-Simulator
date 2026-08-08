//
// Created by xerox on 5/31/2026.
//

#pragma once

#include "hot_entities/EntityManager.hpp"

enum class EntityBroadType : int {
    Character,
    Interactable,
    Projectile,
};

enum class EntityType : int {
    None,
    Player,
    Npc,
    Interactable,
    Enemy,
    Projectile,
};

namespace raylib {
    class Camera2D;
}

class Map;
class GameManager;

class Locals : public ILocals {
public:
    Locals(EntityManager *entityManager,
        class AssetManager *assetManager,
        raylib::Camera2D *screenCamera,
        raylib::Camera2D *worldCamera,
        Map *map,
        GameManager *gameManager)
        : entityManager(entityManager)
        , assetManager(assetManager)
        , screenCamera(screenCamera)
        , worldCamera(worldCamera)
        , map(map)
        , gameManager(gameManager)
    {}
    ~Locals() override = default;

    EntityManager *entityManager;
    AssetManager *assetManager;
    raylib::Camera2D *screenCamera;
    raylib::Camera2D *worldCamera;
    Map *map;
    GameManager *gameManager;
};
