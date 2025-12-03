#pragma once

#include "../ECS/Component.h"
#include <string>
#include <vector>
#include <memory>

namespace Nilos {

/**
 * @brief Simple model loader using Assimp
 * 
 * Loads OBJ and GLTF files into MeshComponent format.
 * Phase 3: Basic loading without animations.
 */
class ModelLoader {
public:
    /**
     * @brief Load a model from file
     * @param filepath Path to model file (OBJ, GLTF, FBX, etc.)
     * @return Vector of mesh components (one per submesh)
     */
    static std::vector<MeshComponent> LoadModel(const std::string& filepath);

    /**
     * @brief Load model and create entity
     * @param filepath Path to model file
     * @param world ECS world to create entity in
     * @return Entity ID of created model
     */
    // static Entity LoadModelAsEntity(const std::string& filepath, World* world);

private:
    /**
     * @brief Process Assimp node recursively
     */
    static void ProcessNode(void* node, void* scene, std::vector<MeshComponent>& meshes);

    /**
     * @brief Convert Assimp mesh to MeshComponent
     */
    static MeshComponent ProcessMesh(void* mesh, void* scene);
};

} // namespace Nilos

