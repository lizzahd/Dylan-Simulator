#include <raylib-cpp.hpp>

#include <hot_assets/AssetManager.h>

#include <Utils.h>
#include <Map.h>
#include <Player.h>
#include <Interactable.h>
#include <Actor.h>
#include <GameManager.h>

#include "Npc.h"

int main() {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    constexpr int width = 1152;
    constexpr int height = 648;
    raylib::Window window(width, height, "Dylan Simulator", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();

    // Assets
    const auto assetManager = std::make_shared<AssetManager>("../..");

    // Rendering
    auto camera = std::make_shared<raylib::Camera2D>(
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2),
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2)
    );

    // Map
    auto map = std::make_shared<Map>("lake");

    // Game State Management
    auto gameManager = std::make_shared<GameManager>();

    // Entity management
    const auto entityManager = std::make_shared<EntityManager>(assetManager, camera, map, gameManager);
    entityManager->registerBroadType(EntityBroadType::Character, typeid(Character), typeid(Actor));

    const auto player = entityManager->create<Player>(raylib::Vector2{300, 500});

    // entityManager->create<Dubi>(raylib::Vector2{200, 550});

    // // Window
    // std::string path = "window";
    // entityManager->create<Interactable>(path, raylib::Vector2{257, 349}, [](ENTITY_REQUIREMENTS) {
    //     gameManager->showDialogue({gameManager, "You see a man in there who is totally [w]naked[r]", 1});
    // });
    //
    // // Service Panel
    // path = "service_panel";
    // entityManager->create<Interactable>(path, raylib::Vector2{480, 354}, [](ENTITY_REQUIREMENTS) {
    //     DialogueText dialogue{gameManager, "Careful, you might get your ass [cFF0000FF]zapped to hell[r]", 1};
    //     dialogue.m_dialogueNodes.push_back(std::make_shared<DialogueNode>(gameManager, "Leave it alone", [](auto g) {
    //         g->closeDialogue();
    //     }));
    //     dialogue.m_dialogueNodes.push_back(std::make_shared<DialogueNode>(gameManager, "Zap your ass to hell", [](auto g) {
    //         g->closeDialogue();
    //     }));
    //     gameManager->showDialogue(dialogue);
    // });

    raylib::RenderTexture viewport = LoadRenderTexture(1000, 1000);
    raylib::Vector2 viewportPos;

    while (!window.ShouldClose()) {
        camera->target = player->m_pos;
        window.BeginDrawing();

        window.ClearBackground(BLACK); // NOLINT

        BeginTextureMode(viewport);
        ClearBackground(BLACK); // NOLINT

        // Control
        const raylib::Vector2 mousePos = camera->GetScreenToWorld(GetMousePosition());
        if (IsKeyPressed(KEY_GRAVE)) {
            inDebugMode = !inDebugMode;
        }

        // Map
        map->drawBackgroundLayers();
        if (inDebugMode) {
            map->drawDebug();
        }

        // Zero
        raylib::Vector2().DrawCircle(2, RED);

        // Update entities and stuff
        if (!gameManager->m_paused) {
            entityManager->updateAll();
        }
        // entityManager->drawAll();

        // TODO: Optimize!!!
        std::vector<int> entityIds = entityManager->getEntitiesByType<Actor>();
        std::ranges::sort(entityIds, [&](const int a, const int b) {
            const auto *entityA = entityManager->getAs<Actor>(a);
            const auto *entityB = entityManager->getAs<Actor>(b);
            return entityA->m_pos.y < entityB->m_pos.y;
        });
        for (const int id : entityIds) {
            const auto *entity = entityManager->getAs<Actor>(id);
            entity->draw();
        }

        map->drawForegroundLayers();

        // UI Scheisse
        if (inDebugMode) {
            DrawText(std::format("Angle Index: {}", player->getAngleIndex()).c_str(), 0, 0, 14, LIME);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            viewportPos.x -= GetMouseDelta().x;
            viewportPos.y += GetMouseDelta().y;
        }

        gameManager->update();
        gameManager->draw();

        EndTextureMode();

        const float heightRatio = std::min(static_cast<float>(GetScreenHeight()) / VIEWPORT_HEIGHT, static_cast<float>(GetScreenHeight()));
        const float newWidth = VIEWPORT_WIDTH * heightRatio;
        const float newHeight = VIEWPORT_HEIGHT * heightRatio;
        const float imageX = GetScreenWidth() / 2 - newWidth / 2;
        DrawTexturePro(
            viewport.texture,
            raylib::Rectangle{viewportPos.x, viewportPos.y, VIEWPORT_WIDTH, -VIEWPORT_HEIGHT},
            raylib::Rectangle{0, 0, newWidth, static_cast<float>(GetScreenHeight())},
            {0, 0},
            0,
            WHITE
        );
        DrawRectangleLines(imageX, 0, newWidth, newHeight, RED);

        DrawFPS(5, 5);

        window.EndDrawing();
    }

    return 0;
}
