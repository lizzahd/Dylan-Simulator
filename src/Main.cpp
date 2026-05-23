#include <raylib-cpp.hpp>

#include <AssetManager.h>
#include <Utils.h>
#include <EntityManager.h>
#include <Map.h>
#include <Player.h>
#include <Interactable.h>

int main() {
    // Initialize raylib
    SetTraceLogLevel(LOG_WARNING);
    constexpr int width = 1152;
    constexpr int height = 648;
    raylib::Window window(width, height, "RTS Game!!!", FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    SetExitKey(0);
    InitAudioDevice();

    // Assets
    const auto assetManager = std::make_shared<AssetManager>();

    // Rendering
    auto camera = std::make_shared<raylib::Camera2D>(
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2),
        raylib::Vector2(static_cast<float>(width) / 2, static_cast<float>(height) / 2)
    );

    // Map
    auto map = std::make_shared<Map>("../../assets/rooms/house");

    // Entity management
    const auto entityManager = std::make_shared<EntityManager>(assetManager, camera, map);
    const auto player = entityManager->create<Player>(raylib::Vector2{200, 200});
    std::vector<raylib::Vector2> points{
        {259, 351},
        {282, 354},
        {281, 384},
        {260, 383},
    };
    std::string path = "window";
    entityManager->create<Interactable>(path, raylib::Vector2{259, 351}, points);

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

        // Zero
        raylib::Vector2().DrawCircle(2, RED);

        // Update entities and stuff
        entityManager->updateAll();

        map->drawForegroundLayers();

        // UI Scheisse
        if (inDebugMode) {
            DrawText(std::format("Angle Index: {}", player->getAngleIndex()).c_str(), 0, 0, 14, LIME);
        }

        window.EndDrawing();
    }

    return 0;
}
