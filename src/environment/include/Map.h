//
// Created by xerox on 4/25/2026.
//

#pragma once

#include <string>
#include <vector>
#include <raylib-cpp/Texture.hpp>

struct Line;

struct Geometry {
    std::vector<raylib::Vector2> m_vertices;
    std::vector<Line> m_collisionLines;

    void drawDebug() const;
};

class Map {
public:
    ~Map() = default;
    Map() = default;
    explicit Map(const std::string& roomPath);

    void drawBackgroundLayers() const;
    void drawForegroundLayers() const;
    void drawDebug() const;

    std::vector<raylib::Rectangle> m_hitboxes;
    std::vector<raylib::Texture2D> m_backgroundLayers;
    std::vector<raylib::Texture2D> m_foregroundLayers;
    std::vector<raylib::Texture2D> m_backgroundParallaxLayers;

    // Physics and shite
    std::vector<Geometry> m_geometries;
};
