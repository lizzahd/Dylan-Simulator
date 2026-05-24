//
// Created by xerox on 3/13/2026.
//

#pragma once

#include <memory>
#include <ranges>
#include <typeindex>
#include <unordered_map>

#include <IEntity.h>
#include <Actor.h>
#include <Character.h>
#include <AssetManager.h>
#include <Interactable.h>
#include <map>
#include <Utils.h>

class Map;

class EntityManager : public std::enable_shared_from_this<EntityManager> {
public:
    explicit EntityManager(const std::shared_ptr<AssetManager> &assetManager, const std::shared_ptr<Camera2D> &camera, const std::shared_ptr<Map> &map)
        : m_assetManager(assetManager)
        , m_camera(camera)
        , m_map(map)
    {}
    ~EntityManager() = default;

    template<typename T>
    void addEntityByBroadType(T* ptr) {
        // Layer Scheisse
        const int id = ptr->getId();
        const int layer = ptr->getLayer();
        m_layerIndex[layer].push_back(id);

        // Type Scheisse
        switch (ptr->getBroadType()) {
            case EntityBroadType::Character: {
                m_entitiesByType[std::type_index(typeid(Character))].push_back(ptr->getId());
                m_entitiesByType[std::type_index(typeid(Actor))].push_back(ptr->getId());
                break;
            }
            case EntityBroadType::Interactable: {
                m_entitiesByType[std::type_index(typeid(Interactable))].push_back(ptr->getId());
                m_entitiesByType[std::type_index(typeid(Actor))].push_back(ptr->getId());
                break;
            }
            default: {
                m_entitiesByType[std::type_index(typeid(IEntity))].push_back(ptr->getId());
                break;
            }
        }
    }

    template<typename T, typename... Args>
    T* create(Args&&... args) {
        const int id = m_nextId++;
        auto entity = std::make_unique<T>(id, shared_from_this(), m_assetManager, m_camera, m_map, std::forward<Args>(args)...);
        T* ptr = entity.get();
        m_entities[id] = std::unique_ptr<IEntity>(std::move(entity));

        addEntityByBroadType(ptr);

        return ptr;
    }

    template<typename T>
    T* create(std::unique_ptr<T> entity) {
        T* ptr = entity.get();
        m_entities[ptr->getId()] = std::unique_ptr<IEntity>(std::move(entity));

        addEntityByBroadType(ptr);

        return ptr;
    }

    template<typename T, typename... Args>
    std::unique_ptr<T> createObject(Args&&... args) {
        return std::make_unique<T>(m_nextId++, shared_from_this(), m_assetManager, m_camera, m_map, std::forward<Args>(args)...);
    }

    IEntity* get(const int id) {
        const auto it = m_entities.find(id);
        return it != m_entities.end() ? it->second.get() : nullptr;
    }

    template<typename T>
    T* getAs(const int id) {
        return dynamic_cast<T*>(get(id));
    }

    void remove(const int id) {
        const auto it = m_entities.find(id);
        if (it == m_entities.end()) {
            return;
        }

        // Layer Scheisse
        const int layer = it->second->getLayer();
        auto &vec = m_layerIndex[layer];
        std::erase(vec, id);

        m_entities.erase(id);
        for (auto& entityIds : m_entitiesByType | std::views::values) {
            std::erase(entityIds, id);
        }
    }

    void updateAll() {
        std::vector<int> toRemove;
        for (const auto& [id, entity]: m_entities) {
            if (entity->isDead()) {
                toRemove.push_back(id);
                continue;
            }

            entity->update();
            // entity->draw();
            //
            // if (inDebugMode) {
            //     entity->drawDebug();
            // }
        }

        // Gotta draw separately due to layering. A bit expensive
        for (const auto &ids: m_layerIndex | std::views::values) {
            for (const int id : ids) {
                m_entities[id]->draw();

                if (inDebugMode) {
                    m_entities[id]->drawDebug();
                }
            }
        }

        for (const int id : toRemove) {
            remove(id);
        }
    }

    template<typename T>
    std::vector<int>& getEntitiesByType() {
        return m_entitiesByType[std::type_index(typeid(T))];
    }

    template<typename F>
    void exec(F&& cb) {
        for (const auto &[id, entity]: m_entities) {
            cb(id, entity.get());
        }
    }

    template<typename T, typename F>
    void execByType(F&&cb) {
        for (int id : getEntitiesByType<T>()) {
            if (auto* entity = getAs<T>(id)) {
                cb(id, entity);
            }
        }
    }

    template<typename T, typename F>
    bool breakableExecByType(F&&cb) {
        for (int id : getEntitiesByType<T>()) {
            if (auto* entity = getAs<T>(id)) {
                if (cb(id, entity)) {
                    return true;
                }
            }
        }

        return false;
    }

    // Layer Scheisse
    void setLayer(const int id, const int layer) {
        const auto it = m_entities.find(id);
        if (it == m_entities.end()) {
            return;
        }

        const int oldLayer = it->second->getLayer();
        if (oldLayer == layer) {
            return;
        }

        auto &oldVec = m_layerIndex[oldLayer];
        std::erase(oldVec, id);

        it->second->setLayer(layer);
        m_layerIndex[layer].push_back(id);
    }

    int m_nextId = 0;
private:
    std::unordered_map<int, std::unique_ptr<IEntity>> m_entities;

    // This is so I can just find all structures or units really fast
    std::unordered_map<std::type_index, std::vector<int>> m_entitiesByType;

    std::shared_ptr<AssetManager> m_assetManager;
    std::shared_ptr<Camera2D> m_camera;
    std::shared_ptr<Map> m_map;
    std::map<int, std::vector<int>> m_layerIndex;
};
