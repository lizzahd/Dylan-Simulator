//
// Created by xerox on 5/31/2026.
//

#pragma once

/// The types used by entity manager so it doesn't need external libraries
#define ENTITY_MANAGER_DECLARATIONS class Map; \
                                    class AssetManager; \
                                    struct Camera2D; \
                                    class GameManager;

/// The arguments EntityManager needs for its constructor
#define ENTITY_MANAGER_REQUIREMENTS AssetManager *assetManager, \
                                    Camera2D *camera, \
                                    Map *map, \
                                    GameManager *gameManager

/// The values being passed to an entity upon its creation in EntityManager
#define ENTITY_MANAGER_MEMBERS      m_assetManager, \
                                    m_camera, \
                                    m_map, \
                                    m_gameManager

/// The constructor fields for EntityManager
#define ENTITY_MANAGER_CONSTRUCTION : m_assetManager(assetManager) \
                                    , m_camera(camera) \
                                    , m_map(map) \
                                    , m_gameManager(gameManager)

/// The member declarations for EntityManager
#define ENTITY_MANAGER_MEMBERS_DECL AssetManager *m_assetManager; \
                                    Camera2D *m_camera; \
                                    Map *m_map; \
                                    GameManager *m_gameManager;