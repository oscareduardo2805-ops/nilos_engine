#include "ModelLoader.h"
#include "../Core/Logger.h"

// Assimp will be included when vcpkg installation completes
// For now, provide a stub implementation

namespace Nilos {

std::vector<MeshComponent> ModelLoader::LoadModel(const std::string& filepath) {
    std::vector<MeshComponent> meshes;
    
    // TODO: Implement with Assimp once installed
    NILOS_WARNING("Model loading not yet implemented (Assimp installing...)");
    NILOS_WARNING("Requested file: ", filepath);
    
    // Return empty cube as placeholder
    MeshComponent placeholder;
    placeholder.CreateCube();
    meshes.push_back(placeholder);
    
    return meshes;
}

void ModelLoader::ProcessNode(void* node, void* scene, std::vector<MeshComponent>& meshes) {
    // TODO: Implement with Assimp
}

MeshComponent ModelLoader::ProcessMesh(void* mesh, void* scene) {
    // TODO: Implement with Assimp
    MeshComponent component;
    component.CreateCube();
    return component;
}

} // namespace Nilos

