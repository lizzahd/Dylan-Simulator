//
// Created by xerox on 4/25/2026.
//

#include <algorithm>
#include <raylib-cpp/Rectangle.hpp>

#include <AStar.h>
#include <Map.h>
#include <Utils.h>
#include <EntityManager.h>

int getSquaredDistance(const IPos &first, const IPos &second) {
    const int xd2 = static_cast<int>(std::round(pow((first.x - second.x), 2)));
    const int yd2 = static_cast<int>(std::round(pow((first.y - second.y), 2)));
    return (xd2 + yd2);
}

std::vector<IPos> getPathFromNode(std::weak_ptr<Node> node) {
    std::vector<IPos> path;

    while (const auto lock = node.lock()) {
        path.push_back(lock->pos);
        node = lock->parent;
    }

    path.pop_back();
    return path;
}

#define ADJACENT ADJACENT_4
// #define ADJACENT ADJACENT_8

std::vector<IPos> astar(const std::shared_ptr<Map> &map, const std::shared_ptr<EntityManager> &entityManager, const raylib::Vector2 &worldStartPos,
                        const raylib::Vector2 &worldEndPos) {
    // Convert positions from world coords to tile coords
    const IPos startPos(static_cast<int>(worldStartPos.x / CHUNK_SIZE), static_cast<int>(worldStartPos.y / CHUNK_SIZE));
    const IPos endPos(static_cast<int>(worldEndPos.x / CHUNK_SIZE), static_cast<int>(worldEndPos.y / CHUNK_SIZE));
    const auto startNode = std::make_shared<Node>(startPos);
    const auto endNode = std::make_shared<Node>(endPos);

    std::vector<std::shared_ptr<Node>> allNodes; // This is where they live. They don't get removed from this list so they don't go out of scope
    allNodes.push_back(startNode);
    std::vector<std::shared_ptr<Node>> workingNodes; // This contains references to existing nodes
    workingNodes.push_back(allNodes.back());
    std::vector<IPos> visited;

    // TODO: Do distance check to make sure it doesn't hog all our resources
    while (!workingNodes.empty()) {
        int currentIndex = 0;
        for (int i = 0; i < workingNodes.size(); i++) {
            if (workingNodes[i]->cost < workingNodes[currentIndex]->cost) {
                currentIndex = i;
            }
        }

        const auto currentNode = workingNodes[currentIndex];
        visited.push_back(currentNode->pos);
        workingNodes.erase(workingNodes.begin() + currentIndex);

        if (currentNode->pos == endNode->pos) {
            return getPathFromNode(currentNode);
        }

        for (const auto &[ax, ay] : ADJACENT) {
            const IPos newPos(currentNode->pos.x + ax, currentNode->pos.y + ay);

            auto newNode = std::make_shared<Node>(newPos, currentNode);

            // If invalid map position, continue
            // TODO: Fix
            // if (map->checkTile(newPos)) {
            //     continue;
            // }

            // If visited already, continue
            if (std::ranges::find(visited, newPos) != visited.end()) {
                continue;
            }

            newNode->distFromTarget = getSquaredDistance(newNode->pos, endNode->pos);
            newNode->cost = newNode->distFromStart + newNode->distFromTarget;
            const bool found = std::ranges::any_of(workingNodes, [&](const auto node) {
                return newNode->pos == node->pos && newNode->distFromStart > node->distFromStart;
            });
            if (found) continue;

            allNodes.push_back(newNode);
            workingNodes.push_back(allNodes.back());
        }
    }

    return {};
}