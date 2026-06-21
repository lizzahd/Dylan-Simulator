//
// Created by xerox on 6/20/2026.
//

#include <raylib-cpp.hpp>

#include <hot_assets/AssetManager.h>

#include <Utils.h>
#include <Map.h>
#include <Interactable.h>
#include <Actor.h>
#include <GameManager.h>

#include "GauntletCharacter.h"

int main() {
    #pragma region RaylibInit
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    constexpr int width = 1152;
    constexpr int height = 648;
    raylib::Window window(width, height, "Dylan Simulator (Gauntlet)", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();
    #pragma endregion RaylibInit

    #pragma region GameInit
    // Assets
    AssetManager assetManager;
    assetManager.loadTextures("../../assets/gauntlet/textures");

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
    entityManager.registerBroadType(EntityBroadType::Character, typeid(gauntlet::Character), typeid(core::Actor));
    #pragma endregion GameInit

    while (!window.ShouldClose()) {
        #pragma region GameDrawing
        window.BeginDrawing();
        window.ClearBackground(BLACK); // NOLINT

        // Map
        map.drawBackgroundLayers();

        entityManager.updateAll();
        entityManager.drawAll();

        map.drawForegroundLayers();
        #pragma endregion GameDrawing

        window.EndDrawing();
    }

    return 0;
}
