//
// Created by xerox on 6/28/2026.
//

/// !!! ATTENTION !!!
/// THIS FILE/TARGET IS FOR QUICK TESTING!
/// NONE OF THE THINGS IN THIS FILE WILL BE INCLUDED IN THE BASE GAME!
///
#include <Combat.h>

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

#define ASSETS_DIR "../../../assets/"

int main() {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    constexpr int width = 1152;
    constexpr int height = 648;
    raylib::Window window(width, height, "Dylan Simulator (Combat)", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();

    // Assets
    AssetManager assetManager;
    // TODO: Figure out good path
    assetManager.loadTextures(ASSETS_DIR"gauntlet/textures");

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

    while (!window.ShouldClose()) {
        window.BeginDrawing();
        window.ClearBackground(BLACK); // NOLINT

        // Map
        map.drawBackgroundLayers();

        entityManager.updateAll();
        entityManager.drawAll();

        map.drawForegroundLayers();

        window.EndDrawing();
    }

    return 0;
}
