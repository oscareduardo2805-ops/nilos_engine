#include "Pathfinding.h"
#include "../Core/Logger.h"
#include <algorithm>
#include <cmath>

namespace Nilos {

std::vector<glm::vec3> Pathfinding::FindPath(const glm::vec3& start, const glm::vec3& goal,
                                               const std::vector<glm::vec3>& obstacles) {
    // Convert world positions to grid
    int startX, startZ, goalX, goalZ;
    WorldToGrid(start, startX, startZ);
    WorldToGrid(goal, goalX, goalZ);

    Node startNode(startX, startZ);
    Node goalNode(goalX, goalZ);

    // Priority queue for open list (min-heap by F score)
    auto compare = [](const Node* a, const Node* b) { return a->F > b->F; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> openList(compare);

    // Closed list (visited nodes)
    std::unordered_map<Node, bool, NodeHash> closedList;

    // All nodes (for memory management)
    std::vector<Node*> allNodes;

    Node* start_ptr = new Node(startNode);
    allNodes.push_back(start_ptr);
    openList.push(start_ptr);

    Node* goalFound = nullptr;

    // A* main loop
    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        // Check if we reached the goal
        if (current->X == goalNode.X && current->Z == goalNode.Z) {
            goalFound = current;
            break;
        }

        closedList[*current] = true;

        // Check neighbors
        std::vector<Node> neighbors = GetNeighbors(*current);
        for (const Node& neighbor : neighbors) {
            // Skip if blocked or already visited
            if (IsBlocked(neighbor.X, neighbor.Z, obstacles) || closedList[neighbor]) {
                continue;
            }

            float tentativeG = current->G + 1.0f; // Cost to move to neighbor

            // Create new node or find existing
            Node* neighborNode = nullptr;
            for (Node* node : allNodes) {
                if (node->X == neighbor.X && node->Z == neighbor.Z) {
                    neighborNode = node;
                    break;
                }
            }

            if (!neighborNode) {
                neighborNode = new Node(neighbor.X, neighbor.Z);
                allNodes.push_back(neighborNode);
            }

            // Update if this path is better
            if (neighborNode->G == 0 || tentativeG < neighborNode->G) {
                neighborNode->G = tentativeG;
                neighborNode->H = Heuristic(*neighborNode, goalNode);
                neighborNode->F = neighborNode->G + neighborNode->H;
                neighborNode->Parent = current;
                openList.push(neighborNode);
            }
        }
    }

    // Reconstruct path
    std::vector<glm::vec3> path;
    if (goalFound) {
        Node* current = goalFound;
        while (current) {
            path.push_back(GridToWorld(current->X, current->Z));
            current = current->Parent;
        }
        std::reverse(path.begin(), path.end());
    }

    // Cleanup
    for (Node* node : allNodes) {
        delete node;
    }

    return path;
}

void Pathfinding::WorldToGrid(const glm::vec3& world, int& gridX, int& gridZ) const {
    gridX = static_cast<int>(std::floor(world.x / m_CellSize));
    gridZ = static_cast<int>(std::floor(world.z / m_CellSize));
}

glm::vec3 Pathfinding::GridToWorld(int gridX, int gridZ) const {
    return glm::vec3(gridX * m_CellSize + m_CellSize * 0.5f,
                     0.0f,
                     gridZ * m_CellSize + m_CellSize * 0.5f);
}

float Pathfinding::Heuristic(const Node& a, const Node& b) const {
    // Manhattan distance
    return static_cast<float>(std::abs(a.X - b.X) + std::abs(a.Z - b.Z));
}

std::vector<Pathfinding::Node> Pathfinding::GetNeighbors(const Node& node) const {
    std::vector<Node> neighbors;
    
    // 4-directional (up, down, left, right)
    neighbors.emplace_back(node.X + 1, node.Z);
    neighbors.emplace_back(node.X - 1, node.Z);
    neighbors.emplace_back(node.X, node.Z + 1);
    neighbors.emplace_back(node.X, node.Z - 1);

    return neighbors;
}

bool Pathfinding::IsBlocked(int gridX, int gridZ, const std::vector<glm::vec3>& obstacles) const {
    glm::vec3 worldPos = GridToWorld(gridX, gridZ);
    
    // Check if this grid cell overlaps with any obstacle
    for (const glm::vec3& obstacle : obstacles) {
        int obsX, obsZ;
        WorldToGrid(obstacle, obsX, obsZ);
        if (obsX == gridX && obsZ == gridZ) {
            return true;
        }
    }
    
    return false;
}

} // namespace Nilos

