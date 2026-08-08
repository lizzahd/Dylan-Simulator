//
// Created by xerox on 3/21/2026.
//

#pragma once

#include <raylib-cpp.hpp>

#include <Map.h>
#include <EntityTypes.h>

#include <hot_entities/IEntity.hpp>

namespace raylib {
    class Rectangle;
}

class AssetManager;

namespace core {
    class Actor : public IEntity {
    public:
        explicit Actor(
            const int id,
            ILocals *iLocals,
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
            , m_iLocals(iLocals) {
            auto *locals = dynamic_cast<Locals *>(iLocals);
            m_entityManager = (EntityManager *) locals->entityManager; // NOLINT Why do I need to c-cast this shit? What the hell?
            m_assetManager = locals->assetManager;
            m_screenCamera = locals->screenCamera;
            m_worldCamera = locals->worldCamera;
            m_map = locals->map;
            m_gameManager = locals->gameManager;
        }

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

        [[nodiscard]] int getType() const override {
            return static_cast<int>(m_entityType);
        }

        [[nodiscard]] int getBroadType() const override {
            return static_cast<int>(m_entityBroadType);
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
        EntityManager *m_entityManager;
        AssetManager *m_assetManager;
        raylib::Camera2D *m_screenCamera;
        raylib::Camera2D *m_worldCamera;
        Map *m_map;
        GameManager *m_gameManager;
        ILocals *m_iLocals;
        int m_layer = 1;
    };
}