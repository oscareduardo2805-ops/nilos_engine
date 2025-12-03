#pragma once

#include "../Physics/Collision.h"
#include "../AI/Pathfinding.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Nilos {

/**
 * @brief Debug rendering utilities (Phase 3)
 * 
 * Visualize physics, pathfinding, and debug info.
 * Simple wireframe rendering.
 */
class DebugRenderer {
public:
    DebugRenderer() = default;
    ~DebugRenderer() = default;

    /**
     * @brief Initialize debug renderer
     */
    bool Initialize();

    /**
     * @brief Draw AABB wireframe
     */
    void DrawAABB(const AABB& aabb, const glm::vec3& color = glm::vec3(0.0f, 1.0f, 0.0f));

    /**
     * @brief Draw line
     */
    void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f));

    /**
     * @brief Draw path (series of lines)
     */
    void DrawPath(const std::vector<glm::vec3>& path, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 0.0f));

    /**
     * @brief Render all queued debug shapes
     */
    void Render(const glm::mat4& view, const glm::mat4& projection);

    /**
     * @brief Clear all debug shapes
     */
    void Clear();

    /**
     * @brief Cleanup
     */
    void Shutdown();

private:
    struct DebugLine {
        glm::vec3 Start;
        glm::vec3 End;
        glm::vec3 Color;
    };

    std::vector<DebugLine> m_Lines;
    uint32_t m_VAO = 0;
    uint32_t m_VBO = 0;
    std::unique_ptr<Shader> m_Shader;
};

} // namespace Nilos

