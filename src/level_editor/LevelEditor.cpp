//
// Created by xerox on 6/19/2026.
//

#define ASSETS_DIR "../../../assets/"
#define ROOMS_DIR ASSETS_DIR"rooms/"

#define SNAP_DIST 5
#define HOVER_DIST 5

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
    AddLineCollision,

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
        case EditMode::AddLineCollision:
            return "Current Mode: Add Line Collision";
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
    assetManager.loadTextures(ASSETS_DIR"textures");
    assetManager.loadShaders(ASSETS_DIR"shaders");

    // Rendering
    raylib::Camera2D camera(
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2),
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2)
    );

    // Map
    Map map;
    // map.m_currentRoom = "current";
    // map.m_rooms.emplace("current", std::make_shared<Room>());

    // Game State Management
    GameManager gameManager;

    // Entity management
    EntityManager entityManager(&assetManager, &camera, &map, &gameManager);
    entityManager.registerBroadType(EntityBroadType::Character, typeid(Character), typeid(Actor));
    entityManager.registerBroadType(EntityBroadType::Interactable, typeid(Interactable), typeid(Actor));

    map.load(ROOMS_DIR"lake", &entityManager, &gameManager, true);
    #pragma endregion GameInit

    #pragma region EditorInit
    auto selectLayer = SelectLayer::Entity;
    auto editMode = EditMode::None;

    std::optional<std::unique_ptr<Actor>> entityPaintbrush;
    std::optional<raylib::Vector2> previousPoint;
    std::optional<raylib::Vector2> snapPoint;

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
            previousPoint = std::nullopt;
        }
        if (IsKeyPressed(KEY_R)) {
            editMode = EditMode::AddRectCollision;
            selectLayer = SelectLayer::Collision;
            previousPoint = std::nullopt;
        }
        if (IsKeyPressed(KEY_C)) {
            editMode = EditMode::AddCircleCollision;
            selectLayer = SelectLayer::Collision;
            previousPoint = std::nullopt;
        }
        if (IsKeyPressed(KEY_N)) {
            editMode = EditMode::AddLineCollision;
            selectLayer = SelectLayer::Collision;
            previousPoint = std::nullopt;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            editMode = EditMode::None;
            entityPaintbrush = std::nullopt;
            previousPoint = std::nullopt;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            entityPaintbrush = std::nullopt;
            previousPoint = std::nullopt;
        }
        #pragma endregion ControlModes

        const raylib::Vector2 mouseDelta = GetMouseDelta();
        const raylib::Vector2 mousePos = GetMousePosition();
        const raylib::Vector2 mouseWorldPos = camera.GetScreenToWorld(mousePos);
        const auto currentRoom = map.getCurrentRoom();

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

        auto editorPos = mouseWorldPos;
        if (snapPoint.has_value()) {
            editorPos = snapPoint.value();
        }

        switch (selectLayer) {
            case SelectLayer::Entity: {
                if (entityPaintbrush.has_value()) {
                    const auto &entity = entityPaintbrush.value();
                    entity->m_pos = mouseWorldPos;
                    entity->draw();

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        entityManager.create(std::move(entityPaintbrush.value()));
                        entityPaintbrush = std::nullopt;
                    }
                }
                break;
            }
            case SelectLayer::Collision: {
                switch (editMode) {
                    case EditMode::AddRectCollision: {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            if (previousPoint.has_value()) {
                                auto pos = previousPoint.value();
                                auto size = editorPos - pos;
                                if (size.x < 0) {
                                    pos.x += size.x;
                                    size.x *= -1;
                                }
                                if (size.y < 0) {
                                    pos.y += size.y;
                                    size.y *= -1;
                                }
                                map.getCurrentRoom()->m_collisionRects.emplace_back(pos.x, pos.y, size.x, size.y);
                                previousPoint = std::nullopt;
                            } else {
                                previousPoint = editorPos;
                            }
                        }
                        break;
                    }
                    case EditMode::AddCircleCollision: {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            if (const auto point = previousPoint) {
                                map.getCurrentRoom()->m_collisionCircles.emplace_back(point.value(), point->Distance(editorPos));
                                previousPoint = std::nullopt;
                            } else {
                                previousPoint = editorPos;
                            }
                        }
                        break;
                    }
                    case EditMode::AddLineCollision: {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            if (const auto point = previousPoint) {
                                map.getCurrentRoom()->m_collisionLines.emplace_back(point.value(), editorPos);
                                previousPoint = std::nullopt;
                            } else {
                                previousPoint = editorPos;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }

        map.drawForegroundLayers();
        map.drawDebug();

        if (const auto point = previousPoint) {
            switch (editMode) {
                case EditMode::AddRectCollision: {
                    auto pos = point.value();
                    auto size = editorPos - pos;
                    if (size.x < 0) {
                        pos.x += size.x;
                        size.x *= -1;
                    }
                    if (size.y < 0) {
                        pos.y += size.y;
                        size.y *= -1;
                    }
                    DrawRectangleLinesEx({pos.x, pos.y, size.x, size.y}, 2, RED);
                    break;
                }
                case EditMode::AddCircleCollision: {
                    DrawCircleLinesV(point.value(), point->Distance(editorPos), RED);
                    break;
                }
                case EditMode::AddLineCollision: {
                    point->DrawLine(editorPos, 2, RED);
                    break;
                }
                default:
                    break;
            }
        }

        // Highlight and be able to delete things
        bool snapPointFound = false;
        int i = 0;
        for (const auto [a, b] : currentRoom->m_collisionLines) {
            if (a.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = a;
                snapPointFound = true;
            }
            if (b.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = b;
                snapPointFound = true;
            }

            if (editMode == EditMode::None) {
                if (CheckCollisionCircleLine(mouseWorldPos, HOVER_DIST, a, b)) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        currentRoom->m_collisionLines.erase(currentRoom->m_collisionLines.begin() + i);
                        break;
                    }

                    a.DrawLine(b, 2.5, WHITE);
                    break;
                }
            }
            i++;
        }

        i = 0;
        for (const auto [pos, radius] : currentRoom->m_collisionCircles) {
            if (pos.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = pos;
                snapPointFound = true;
            }

            if (editMode == EditMode::None) {
                if (CheckCollisionPointCircle(mouseWorldPos, pos, radius)) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        currentRoom->m_collisionCircles.erase(currentRoom->m_collisionCircles.begin() + i);
                        break;
                    }

                    DrawCircleLinesV(pos, radius, WHITE);
                    break;
                }
            }

            i++;
        }

        i = 0;
        for (const auto rect : currentRoom->m_collisionRects) {
            const raylib::Vector2 a = rect.GetPosition();
            const raylib::Vector2 b = {rect.x + rect.width, rect.y};
            const raylib::Vector2 c = a + rect.GetSize();
            const raylib::Vector2 d = {rect.x, rect.y + rect.height};
            if (a.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = a;
                snapPointFound = true;
            }
            if (b.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = b;
                snapPointFound = true;
            }
            if (c.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = c;
                snapPointFound = true;
            }
            if (d.CheckCollision(mouseWorldPos, SNAP_DIST)) {
                snapPoint = d;
                snapPointFound = true;
            }

            if (editMode == EditMode::None) {
                if (rect.CheckCollision(mouseWorldPos)) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        currentRoom->m_collisionRects.erase(currentRoom->m_collisionRects.begin() + i);
                        break;
                    }

                    rect.DrawLines(WHITE, 2.5);
                    break;
                }
            }

            i++;
        }

        if (!snapPointFound) {
            snapPoint = std::nullopt;
        }

        if (snapPoint.has_value()) {
            snapPoint->DrawCircle(2, GREEN);
        }

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