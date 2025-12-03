#pragma once

#include <glm/glm.hpp>

namespace Nilos {

/**
 * @brief Directional light (sun-like)
 * 
 * Infinite distance light with parallel rays.
 * Most performant lighting type.
 */
struct DirectionalLight {
    glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
    float Intensity = 1.0f;
};

/**
 * @brief Point light (light bulb)
 * 
 * Emits light in all directions from a point.
 */
struct PointLight {
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Color = glm::vec3(1.0f);
    float Intensity = 1.0f;
    
    // Attenuation (distance falloff)
    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;
};

/**
 * @brief Ambient light (global illumination approximation)
 */
struct AmbientLight {
    glm::vec3 Color = glm::vec3(0.2f, 0.2f, 0.3f);
    float Intensity = 0.3f;
};

} // namespace Nilos

