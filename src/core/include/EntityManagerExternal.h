//
// Created by xerox on 5/31/2026.
//

#pragma once

namespace raylib {
    class Camera2D;
}

/// The struct of stuff all entities share
struct EntityResources {
    ~EntityResources() = default;
    raylib::Camera2D *screenCamera;
    raylib::Camera2D *worldCamera;
};

/// The types used by entity manager so it doesn't need external libraries
#define ENTITY_MANAGER_DECLARATIONS class AssetManager; \
                                    class Map; \
                                    class GameManager; \
                                    struct EntityResources;

/// The arguments EntityManager needs for its constructor
#define ENTITY_MANAGER_REQUIREMENTS AssetManager *assetManager, \
                                    EntityResources *res, \
                                    Map *map, \
                                    GameManager *gameManager

/// The values being passed to an entity upon its creation in EntityManager
#define ENTITY_MANAGER_MEMBERS      m_assetManager, \
                                    m_res, \
                                    m_map, \
                                    m_gameManager

/// The constructor fields for EntityManager
#define ENTITY_MANAGER_CONSTRUCTION : m_assetManager(assetManager) \
                                    , m_res(res) \
                                    , m_map(map) \
                                    , m_gameManager(gameManager)

/// The member declarations for EntityManager
#define ENTITY_MANAGER_MEMBERS_DECL AssetManager *m_assetManager; \
                                    EntityResources *m_res; \
                                    Map *m_map; \
                                    GameManager *m_gameManager;
