//
// Created by xerox on 4/25/2026.
//

#include <algorithm>
#include <raylib-cpp.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include <Map.h>
#include <Utils.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

void Geometry::drawDebug() const {
    DrawLineStrip(m_vertices.data(), m_vertices.size(), BLUE);

    for (const auto &[pos1, pos2] : m_collisionLines) {
        pos1.DrawLine(pos2, 3, RED);
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

    // Load geometry
    std::ifstream f(roomPath + "/geometry.json");
    for (const auto &g : json::parse(f)) {
        const auto &points = g["points"];
        std::vector<raylib::Vector2> rPoints;
        for (const auto &p : points) {
            rPoints.emplace_back(p["x"], p["y"]);
        }

        std::vector<Line> rCollisionLines;
        const auto &collisionLines = g["collisionLines"];
        for (const auto &line : collisionLines) {
            const raylib::Vector2 pos1(line["a"]["x"], line["a"]["y"]);
            const raylib::Vector2 pos2(line["b"]["x"], line["b"]["y"]);
            rCollisionLines.emplace_back(pos1, pos2);
        }

        m_geometries.emplace_back(rPoints, rCollisionLines);
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
    for (const auto &geometry : m_geometries) {
        geometry.drawDebug();
    }

    for (const auto hitbox : m_hitboxes) {
        hitbox.DrawLines(RED);
    }
}
