#pragma once

#include "../ECS/Component.h"
#include "Shader.h"
#include "Light.h"
#include <memory>

namespace Nilos {

/**
 * @brief Main rendering system
 * 
 * Coordinates the rendering pipeline:
 * - Manages shaders
 * - Uploads mesh data to GPU
 * - Renders entities with mesh components
 * - Handles render state
 * 
 * Future enhancements:
 * - Render queue sorting (by material, distance, etc.)
 * - Multi-pass rendering (shadows, post-processing)
 * - Instanced rendering
 * - Frustum culling
 */
class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    /**
     * @brief Initialize the renderer
     */
    bool Initialize();

    /**
     * @brief Shutdown and cleanup
     */
    void Shutdown();

    /**
     * @brief Begin a new frame
     */
    void BeginFrame();

    /**
     * @brief End the current frame
     */
    void EndFrame();

    /**
     * @brief Render a mesh with transform and camera
     */
    void RenderMesh(MeshComponent& mesh, 
                    const TransformComponent& transform,
                    const CameraComponent& camera,
                    const TransformComponent& cameraTransform);

    /**
     * @brief Set clear color
     */
    void SetClearColor(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Set directional light (sun)
     */
    void SetDirectionalLight(const DirectionalLight& light) { m_DirectionalLight = light; }

    /**
     * @brief Set ambient light
     */
    void SetAmbientLight(const AmbientLight& light) { m_AmbientLight = light; }

private:
    /**
     * @brief Initialize mesh buffers (VAO, VBO, EBO)
     */
    void InitializeMeshBuffers(MeshComponent& mesh);

    std::unique_ptr<Shader> m_PhongShader;
    glm::vec4 m_ClearColor;
    
    // Lighting
    DirectionalLight m_DirectionalLight;
    AmbientLight m_AmbientLight;
};

} // namespace Nilos

