//
// Created by xerox on 4/25/2026.
//

#pragma once

#include <memory>
#include <vector>
#include <raylib-cpp/Vector2.hpp>
#include <Map.h>
#include <Utils.h>

namespace core {
    class EntityManager;
    static int nodeId = 0;

    struct Node : std::enable_shared_from_this<Node> {
        ~Node() = default;

        explicit Node(const IPos &pos)
            : pos(pos)
            , distFromStart(0)
            , distFromTarget(0)
            , cost(0)
        {}

        explicit Node(const IPos &pos, const std::shared_ptr<Node> &parent)
            : parent(parent)
            , pos(pos)
            , distFromStart(parent->distFromStart + 1)
            , distFromTarget(0)
            , cost(0)
        {}

        std::weak_ptr<Node> parent;

        IPos pos;
        int distFromStart;
        int distFromTarget;
        int cost;
    };

    std::vector<IPos> astar(const std::shared_ptr<Map> &map, const std::shared_ptr<EntityManager> &entityManager, const raylib::Vector2 &worldStartPos, const raylib::Vector2 &worldEndPos);
}