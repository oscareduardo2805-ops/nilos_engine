#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>

namespace Nilos {

/**
 * @brief Simple grid-based A* pathfinding
 * 
 * Lightweight pathfinding for NPCs.
 * Uses a 2D grid projected on XZ plane (Y is ignored).
 */
class Pathfinding {
public:
    struct Node {
        int X, Z;
        float G, H, F; // G = cost from start, H = heuristic, F = G + H
        Node* Parent;

        Node(int x, int z) : X(x), Z(z), G(0), H(0), F(0), Parent(nullptr) {}

        bool operator==(const Node& other) const {
            return X == other.X && Z == other.Z;
        }
    };

    struct NodeHash {
        size_t operator()(const Node& node) const {
            return std::hash<int>()(node.X) ^ (std::hash<int>()(node.Z) << 1);
        }
    };

    Pathfinding(float cellSize = 1.0f) : m_CellSize(cellSize) {}

    /**
     * @brief Find path from start to goal
     * @param start Starting world position
     * @param goal Goal world position
     * @param obstacles List of obstacle positions (world space)
     * @return List of waypoints (world space)
     */
    std::vector<glm::vec3> FindPath(const glm::vec3& start, const glm::vec3& goal,
                                     const std::vector<glm::vec3>& obstacles = {});

    /**
     * @brief Set grid cell size
     */
    void SetCellSize(float size) { m_CellSize = size; }
    float GetCellSize() const { return m_CellSize; }

private:
    float m_CellSize;

    /**
     * @brief Convert world position to grid coordinates
     */
    void WorldToGrid(const glm::vec3& world, int& gridX, int& gridZ) const;

    /**
     * @brief Convert grid coordinates to world position
     */
    glm::vec3 GridToWorld(int gridX, int gridZ) const;

    /**
     * @brief Heuristic function (Manhattan distance)
     */
    float Heuristic(const Node& a, const Node& b) const;

    /**
     * @brief Get neighboring grid cells
     */
    std::vector<Node> GetNeighbors(const Node& node) const;

    /**
     * @brief Check if a grid cell is blocked
     */
    bool IsBlocked(int gridX, int gridZ, const std::vector<glm::vec3>& obstacles) const;
};

} // namespace Nilos

