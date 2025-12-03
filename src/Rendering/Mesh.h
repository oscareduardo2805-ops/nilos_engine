#pragma once

#include "../ECS/Component.h"

namespace Nilos {

/**
 * @brief Mesh utilities and factory functions
 * 
 * Helper functions to create common mesh shapes.
 * In the future, this will include model loading (GLTF, OBJ, etc.)
 */
namespace MeshFactory {

    /**
     * @brief Create a cube mesh
     */
    MeshComponent CreateCube();

    /**
     * @brief Create a plane mesh
     */
    MeshComponent CreatePlane(float width = 1.0f, float height = 1.0f);

    /**
     * @brief Create a sphere mesh
     */
    MeshComponent CreateSphere(float radius = 1.0f, int segments = 32);

} // namespace MeshFactory

} // namespace Nilos

