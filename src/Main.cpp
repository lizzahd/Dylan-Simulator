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
    auto map = std::make_shared<Map>("../../assets/rooms/house");

    // Game State Management
    auto gameManager = std::make_shared<GameManager>();

    // Entity management
    const auto entityManager = std::make_shared<EntityManager>(assetManager, camera, map, gameManager);
    entityManager->registerBroadType(EntityBroadType::Character, typeid(Character), typeid(Actor));

    const auto player = entityManager->create<Player>(raylib::Vector2{300, 500});

    entityManager->create<Npc>(EntityType::Npc, std::vector{
        Animation(assetManager, "dubi_idle", raylib::Vector2(140, 140), {70, 115}, 5, 9, true),
    }, raylib::Vector2{500, 550}, raylib::Vector2{140, 140});

    // Window
    std::string path = "window";
    entityManager->create<Interactable>(path, raylib::Vector2{259, 351}, [](ENTITY_REQUIREMENTS) {
        gameManager->showDialogue({gameManager, "You see a man in there who is totally [w]naked[r]", 1});
    });

    // Service Panel
    path = "service_panel";
    entityManager->create<Interactable>(path, raylib::Vector2{482, 356}, [](ENTITY_REQUIREMENTS) {
        DialogueText dialogue{gameManager, "Careful, you might get your ass [cFF0000FF]zapped to hell[r]", 1};
        dialogue.m_dialogueNodes.push_back(std::make_shared<DialogueNode>(gameManager, "Leave it alone", [](auto g) {
            g->closeDialogue();
        }));
        dialogue.m_dialogueNodes.push_back(std::make_shared<DialogueNode>(gameManager, "Zap your ass to hell", [](auto g) {
            std::cout << "ZAP ZAP NIGGA\n";
            g->closeDialogue();
        }));
        gameManager->showDialogue(dialogue);
    });

    while (!window.ShouldClose()) {
        camera->target = player->m_pos;
        window.BeginDrawing();

        window.ClearBackground(BLACK); // NOLINT

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
        entityManager->drawAll();

        map->drawForegroundLayers();

        // UI Scheisse
        if (inDebugMode) {
            DrawText(std::format("Angle Index: {}", player->getAngleIndex()).c_str(), 0, 0, 14, LIME);
        }

        gameManager->update();
        gameManager->draw();

        window.EndDrawing();
    }

    return 0;
}
