#include <raylib-cpp.hpp>

#include <hot_entities/EntityManager.hpp>
#include <hot_assets/AssetManager.h>

#include <Utils.h>
#include <Map.h>
#include <Player.h>
#include <Interactable.h>
#include <Actor.h>
#include <GameManager.h>
#include <EntityTypes.h>
#include <Signal.h>

#include "Npc.h"

using namespace core;

int main() {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    int width = 1280;
    int height = 960;
    constexpr int viewportWidth = VIEWPORT_WIDTH;
    constexpr int viewportHeight = VIEWPORT_HEIGHT;
    raylib::Window window(width, height, "Dylan Simulator", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();

    AssetManager assetManager;
    assetManager.loadTextures("../../assets/textures");
    assetManager.loadShaders("../../assets/shaders");

    float virtualRatio = static_cast<float>(width) / static_cast<float>(height);

    // Why does setting everything manually like this work?
    raylib::Camera2D worldCamera;
    worldCamera.offset = Vector2{0, 0};
    worldCamera.target = Vector2{0, 0};
    worldCamera.rotation = 0;
    worldCamera.zoom = 1.0f;

    raylib::Camera2D screenCamera;
    screenCamera.offset = Vector2{0, 0};
    screenCamera.target = Vector2{0, 0};
    screenCamera.rotation = 0;
    screenCamera.zoom = 1.0f;

    raylib::RenderTexture target = LoadRenderTexture(viewportWidth, viewportHeight);
    raylib::Rectangle src = {0.0f, 0.0f, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height)};
    raylib::Vector2 origin = {0.0f, 0.0f};

    // Map
    Map map;

    // Game State Management
    GameManager gameManager;
    gameManager.init();

    // Entity management
    EntityManager entityManager;
    entityManager.registerBroadType(static_cast<int>(EntityBroadType::Character), typeid(Character), typeid(Actor));
    entityManager.registerBroadType(static_cast<int>(EntityBroadType::Interactable), typeid(Interactable), typeid(Actor));

    // Initialize locals and entity manager
    Locals locals {
        &entityManager,
        &assetManager,
        &screenCamera,
        &worldCamera,
        &map,
        &gameManager,
    };
    entityManager.init(&locals);

    map.load("snow", &entityManager, &gameManager);

    auto *player = entityManager.create<Player>(raylib::Vector2{300, 500});

    auto *dorian = entityManager.create<Dorian>(raylib::Vector2{200, 550});
    dorian->follow(player->m_id);

    auto *dubi = entityManager.create<Dubi>(raylib::Vector2{200, 550});
    dubi->follow(player->m_id);

    while (!window.ShouldClose()) {
        width = GetScreenWidth();
        height = GetScreenHeight();
        raylib::Rectangle dst = {0, 0, static_cast<float>(width), static_cast<float>(height)};

        const auto [mdx, mdy] = GetMouseDelta();
        if (abs(mdx) > 0.1 || abs(mdy) > 0.1) {
            json input_args;
            input_args["mouse_motion"]["dx"] = mdx;
            input_args["mouse_motion"]["dy"] = mdy;
            SignalManager::input_event.emit(input_args);
        }

        BeginTextureMode(target);
            ClearBackground(BLACK);
            BeginMode2D(worldCamera);
                map.drawBackgroundLayers();

                if (!gameManager.m_paused) {
                    entityManager.updateAll();
                }

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

                map.drawForegroundLayers();

                gameManager.update();

                screenCamera.target.x = player->m_pos.x - static_cast<float>(viewportWidth) / 2;
                screenCamera.target.y = player->m_pos.y - static_cast<float>(viewportHeight) / 2;

                worldCamera.target.x = truncf(screenCamera.target.x);
                screenCamera.target.x -= worldCamera.target.x;
                screenCamera.target.x *= virtualRatio;

                worldCamera.target.y = truncf(screenCamera.target.y);
                screenCamera.target.y -= worldCamera.target.y;
                screenCamera.target.y *= virtualRatio;
            EndMode2D();

            gameManager.draw();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK); // NOLINT
            DrawTexturePro(target.texture, src, dst, origin, 0.0f, WHITE);
            DrawFPS(4, 4);
        EndDrawing();
    }

    return 0;
}
