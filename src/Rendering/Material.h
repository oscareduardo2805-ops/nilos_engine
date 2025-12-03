#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace Nilos {

/**
 * @brief Simple material with Phong lighting
 * 
 * Lightweight material for fast rendering.
 * Supports basic textures and Phong lighting model.
 */
struct Material {
    // Color properties
    glm::vec3 Diffuse = glm::vec3(0.8f);
    glm::vec3 Specular = glm::vec3(0.5f);
    float Shininess = 32.0f;
    
    // Texture IDs (0 = no texture)
    uint32_t DiffuseMap = 0;
    uint32_t SpecularMap = 0;
    uint32_t NormalMap = 0;
    
    bool HasDiffuseMap() const { return DiffuseMap != 0; }
    bool HasSpecularMap() const { return SpecularMap != 0; }
    bool HasNormalMap() const { return NormalMap != 0; }
};

} // namespace Nilos

