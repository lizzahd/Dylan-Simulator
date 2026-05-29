//
// Created by xerox on 4/25/2026.
//

#include <algorithm>
#include <raylib-cpp.hpp>
#include <filesystem>

#include <Map.h>
#include <Utils.h>

namespace fs = std::filesystem;

void Geometry::drawDebug() const {
    for (int i = 0; i < m_vertices.size(); i++) {

    }
}

Map::Map(const std::string& roomPath) {
    // Background Textures
    for (const auto& entry : fs::directory_iterator(roomPath)) {
        fs::path path = entry.path();
        if (path.extension() != ".png") {
            continue;
        }

        const std::string pathStr = path.stem().string();
        const auto tex = LoadTexture(path.string().c_str());
        switch (pathStr[1]) {
            case 'b':
                m_backgroundLayers.push_back(tex);
                break;
            case 'f':
                m_foregroundLayers.push_back(tex);
                break;
            case 'p': // Background Parallax
                m_backgroundParallaxLayers.push_back(tex);
                break;
            case 'P': // Foreground Parallax
                // TODO
                break;
            default:
                break;
        }
    }
}

void Map::drawBackgroundLayers() const {
    for (const auto &layer : m_backgroundLayers) {
        layer.Draw();
    }
}

void Map::drawForegroundLayers() const {
    for (const auto &layer : m_foregroundLayers) {
        layer.Draw();
    }
}

void Map::drawDebug() const {
    if (!inDebugMode) {
        return;
    }

    for (const auto hitbox : m_hitboxes) {
        hitbox.DrawLines(RED);
    }
}
