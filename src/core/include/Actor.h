//
// Created by xerox on 3/21/2026.
//

#pragma once

#include <iostream>
#include <raylib-cpp.hpp>

#include <IEntity.h>
#include <Map.h>

// This is for the entity manager, which always hands these to the entity upon creation,
// so we don't need to worry about it.
// The macro just makes this easier to deal with
#define ENTITY_REQUIREMENTS const int id, \
                            const std::shared_ptr<EntityManager>& entityManager, \
                            const std::shared_ptr<AssetManager>& assetManager, \
                            const std::shared_ptr<Camera2D>& camera, \
                            const std::shared_ptr<Map>& map

#define ENTITY_PARAMETERS   id, \
                            entityManager, \
                            assetManager, \
                            camera, \
                            map

#define ENTITY_MEMBERS      m_id, \
                            m_entityManager, \
                            m_assetManager, \
                            m_camera, \
                            m_map


namespace raylib {
    class Rectangle;
}

class EntityManager;
class AssetManager;

class Actor : public IEntity {
public:
    explicit Actor(
        ENTITY_REQUIREMENTS,
        const EntityBroadType broadEntityType,
        const EntityType entityType,
        const raylib::Vector2 pos,
        const raylib::Vector2 size)
        : IEntity()
        , m_pos(pos)
        , m_size(size)
        , m_entityType(entityType)
        , m_id(id)
        , m_entityBroadType(broadEntityType)
        , m_entityManager(entityManager)
        , m_assetManager(assetManager)
        , m_camera(camera)
        , m_map(map)
    {}

    ~Actor() override = default;

    [[nodiscard]] bool isDead() const override {
        return m_isDead;
    }

    [[nodiscard]] int getLayer() const override {
        return m_layer;
    }

    void setLayer(const int layer) override {
        m_layer = layer;
    }

    [[nodiscard]] int getId() const override {
        return m_id;
    }

    [[nodiscard]] EntityType getType() const override {
        return m_entityType;
    }

    [[nodiscard]] EntityBroadType getBroadType() const override {
        return m_entityBroadType;
    }

    [[nodiscard]] virtual raylib::Rectangle getRect() const {
        return {m_pos, m_size};
    }

    [[nodiscard]] virtual raylib::Vector2 getCenter() const {
        return m_pos + m_size / 2;
    }

    [[nodiscard]] virtual int maxHealth() const {
        return 100;
    }

    [[nodiscard]] virtual bool hasCollision() const {
        return true;
    }

    virtual void command(raylib::Vector2) {};

    void draw() const override;
    void drawDebug() const override;

    raylib::Vector2 m_pos;
    raylib::Vector2 m_size;

    bool m_isDead = false;

    EntityType m_entityType;

    int m_id;
protected:
    EntityBroadType m_entityBroadType;
    std::shared_ptr<EntityManager> m_entityManager;
    std::shared_ptr<AssetManager> m_assetManager;
    std::shared_ptr<Camera2D> m_camera;
    std::shared_ptr<Map> m_map;
    int m_layer = 1;
};
