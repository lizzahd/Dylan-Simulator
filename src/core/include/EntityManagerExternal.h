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
#define ENTITY_MANAGER_REQUIREMENTS const std::shared_ptr<AssetManager>& assetManager, \
                                    const std::shared_ptr<Camera2D>& camera, \
                                    const std::shared_ptr<Map>& map, \
                                    const std::shared_ptr<GameManager>& gameManager

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
#define ENTITY_MANAGER_MEMBERS_DECL std::shared_ptr<AssetManager> m_assetManager; \
                                    std::shared_ptr<Camera2D> m_camera; \
                                    std::shared_ptr<Map> m_map; \
                                    std::shared_ptr<GameManager> m_gameManager;