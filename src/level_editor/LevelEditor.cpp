//
// Created by xerox on 6/19/2026.
//

#include <raylib-cpp.hpp>

#include <hot_assets/AssetManager.h>

#include <Utils.h>
#include <Map.h>
#include <Interactable.h>
#include <Actor.h>
#include <GameManager.h>
#include "Button.h"
#include "Npc.h"
#include "Player.h"

#pragma region SelectLayer
enum class SelectLayer : int {
    Entity,
    Collision,

    MAX,
};

SelectLayer getLayerFromInt(const int layer) {
    switch (layer) {
        case static_cast<int>(SelectLayer::Entity):
            return SelectLayer::Entity;
        case static_cast<int>(SelectLayer::Collision):
            return SelectLayer::Collision;
        default:
            return SelectLayer::Entity;
    }
}

std::string_view getLayerName(const SelectLayer layer) {
    switch (layer) {
        case SelectLayer::Entity:
            return "Current Layer: Entity";
        case SelectLayer::Collision:
            return "Current Layer: Collision";
        default:
            return "<Current Layer Not Defined>";
    }
}
#pragma endregion SelectLayer

#pragma region EditMode
enum class EditMode : int {
    None = 0,
    AddEntity,
    AddRectCollision,
    AddCircleCollision,

    MAX,
};

std::string_view getEditModeName(const EditMode mode) {
    switch (mode) {
        case EditMode::None:
            return "Current Mode: None";
        case EditMode::AddEntity:
            return "Current Mode: Add Entity";
        case EditMode::AddRectCollision:
            return "Current Mode: Add Rectangle Collision";
        case EditMode::AddCircleCollision:
            return "Current Mode: Add Circle Collision";
        default:
            return "<Current Mode Not Defined>";
    }
}
#pragma endregion EditMode

enum class ActorType {
    Player,
    Dubi,
};

using namespace core;

int main() {
    #pragma region RaylibInit
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    constexpr int width = 1152;
    constexpr int height = 648;
    raylib::Window window(width, height, "Dylan Simulator (Level Editor)", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();
    #pragma endregion RaylibInit

    #pragma region GameInit
    // Assets
    AssetManager assetManager;
    assetManager.loadTextures("../../assets/textures");
    assetManager.loadShaders("../../assets/shaders");

    // Rendering
    raylib::Camera2D camera(
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2),
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2)
    );

    // Map
    Map map;
    map.m_currentRoom = "current";
    map.m_rooms.emplace("current", std::make_shared<Room>());

    // Game State Management
    GameManager gameManager;

    // Entity management
    EntityManager entityManager(&assetManager, &camera, &map, &gameManager);
    entityManager.registerBroadType(EntityBroadType::Character, typeid(Character), typeid(Actor));
    entityManager.registerBroadType(EntityBroadType::Interactable, typeid(Interactable), typeid(Actor));
    #pragma endregion GameInit

    #pragma region EditorInit
    auto selectLayer = SelectLayer::Entity;
    auto editMode = EditMode::None;

    bool mouseInMapArea = true;

    std::optional<std::unique_ptr<Actor>> entityPaintbrush;

    std::vector<Button> buttons;
    const auto makeActorEntityButton = [&](const int index, const std::string_view text, const std::function<void()> &callback) {
        const float x = static_cast<float>(index) * 100;
        buttons.emplace_back(raylib::Rectangle{x, 0, 100, 50}, text, callback);
    };
    makeActorEntityButton(0, "Player", [&] {
        selectLayer = SelectLayer::Entity;
        editMode = EditMode::AddEntity;
        entityPaintbrush = entityManager.createObject<Player>(raylib::Vector2(0, 0));
    });
    makeActorEntityButton(1, "Dubi", [&] {
        selectLayer = SelectLayer::Entity;
        editMode = EditMode::AddEntity;
        entityPaintbrush = entityManager.createObject<Dubi>(raylib::Vector2(0, 0));
    });
    #pragma endregion EditorInit

    while (!window.ShouldClose()) {
        #pragma region ControlModes

        // Select Layer
        if (IsKeyPressed(KEY_S)) {
            const int nextLayer = static_cast<int>(selectLayer) + 1;
            if (nextLayer >= static_cast<int>(SelectLayer::MAX)) {
                selectLayer = getLayerFromInt(0);
            } else {
                selectLayer = getLayerFromInt(nextLayer);
            }
        }

        // Edit Mode
        if (IsKeyPressed(KEY_E)) {
            editMode = EditMode::AddEntity;
            selectLayer = SelectLayer::Entity;
        }
        if (IsKeyPressed(KEY_R)) {
            editMode = EditMode::AddRectCollision;
            selectLayer = SelectLayer::Collision;
        }
        if (IsKeyPressed(KEY_C)) {
            editMode = EditMode::AddCircleCollision;
            selectLayer = SelectLayer::Collision;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            editMode = EditMode::None;
            entityPaintbrush = std::nullopt;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            entityPaintbrush = std::nullopt;
        }
        #pragma endregion ControlModes

        mouseInMapArea = true;
        const raylib::Vector2 mouseDelta = GetMouseDelta();

        #pragma region GameDrawing
        window.BeginDrawing();
        window.ClearBackground(BLACK); // NOLINT

        camera.BeginMode();
        // Map
        map.drawBackgroundLayers();

        // Zero
        raylib::Vector2().DrawCircle(2, RED);

        // TODO: Optimize!!!
        std::vector<int> entityIds = entityManager.getEntitiesByType<Actor>();
        std::ranges::sort(entityIds, [&](const int a, const int b) {
            const auto *entityA = entityManager.getAs<Actor>(a);
            const auto *entityB = entityManager.getAs<Actor>(b);
            return entityA->m_pos.y < entityB->m_pos.y;
        });
        for (const int id : entityIds) {
            const auto *entity = entityManager.getAs<Actor>(id);
            entity->draw();
        }

        if (entityPaintbrush.has_value() && selectLayer == SelectLayer::Entity) {
            const auto &entity = entityPaintbrush.value();
            entity->m_pos = camera.GetScreenToWorld(GetMousePosition());
            entity->draw();

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                entityManager.create(std::move(entityPaintbrush.value()));
                entityPaintbrush = std::nullopt;
            }
        }

        map.drawForegroundLayers();
        camera.EndMode();
        #pragma endregion GameDrawing

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            camera.target.x -= mouseDelta.x / camera.zoom;
            camera.target.y -= mouseDelta.y / camera.zoom;
        }

        if (IsKeyDown(KEY_LEFT_CONTROL)) {
            if (IsKeyPressed(KEY_N)) {
                // New map
            }

            // Zoom
            camera.zoom += GetMouseWheelMove() / 5;
        }

        #pragma region Buttons
        for (Button &button : buttons) {
            button.update();
            button.draw();

            if (button.isHovered()) {
                mouseInMapArea = false;
                break;
            }
        }
        #pragma endregion Buttons

        #pragma region EditorUI
        DrawText(getLayerName(selectLayer).data(), 5, 5, 16, LIME);
        DrawText(getEditModeName(editMode).data(), 5, 30, 16, LIME);
        #pragma endregion EditorUI

        window.EndDrawing();
    }

    return 0;
}