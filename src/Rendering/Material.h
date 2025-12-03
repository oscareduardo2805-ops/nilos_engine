#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace Nilos {

// Forward declaration
class Shader;

/**
 * @brief Material properties (Placeholder for Phase 2)
 * 
 * In Phase 2, this will be expanded to include:
 * - PBR properties (metallic, roughness, etc.)
 * - Texture maps (albedo, normal, metallic, roughness, AO)
 * - Shader variants
 */
struct Material {
    glm::vec3 Albedo = glm::vec3(1.0f);
    float Metallic = 0.0f;
    float Roughness = 0.5f;
    float AO = 1.0f; // Ambient Occlusion
    
    // Future: texture IDs
    // uint32_t AlbedoMap = 0;
    // uint32_t NormalMap = 0;
    // uint32_t MetallicMap = 0;
    // uint32_t RoughnessMap = 0;
};

} // namespace Nilos

