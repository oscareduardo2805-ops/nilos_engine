#include "Mesh.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Nilos {
namespace MeshFactory {

MeshComponent CreateCube() {
    MeshComponent mesh;
    mesh.CreateCube();
    return mesh;
}

MeshComponent CreatePlane(float width, float height) {
    MeshComponent mesh;
    
    float halfW = width * 0.5f;
    float halfH = height * 0.5f;
    
    // Plane vertices (lying on XZ plane)
    mesh.Vertices = {
        // Position              // Color
        -halfW, 0.0f, -halfH,    0.8f, 0.8f, 0.8f,
         halfW, 0.0f, -halfH,    0.8f, 0.8f, 0.8f,
         halfW, 0.0f,  halfH,    0.8f, 0.8f, 0.8f,
        -halfW, 0.0f,  halfH,    0.8f, 0.8f, 0.8f,
    };
    
    mesh.Indices = {
        0, 1, 2,
        2, 3, 0
    };
    
    return mesh;
}

MeshComponent CreateSphere(float radius, int segments) {
    MeshComponent mesh;
    
    // Generate sphere vertices
    for (int lat = 0; lat <= segments; ++lat) {
        float theta = static_cast<float>(lat) * static_cast<float>(M_PI) / static_cast<float>(segments);
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);
        
        for (int lon = 0; lon <= segments; ++lon) {
            float phi = static_cast<float>(lon) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            
            // Position
            mesh.Vertices.push_back(x * radius);
            mesh.Vertices.push_back(y * radius);
            mesh.Vertices.push_back(z * radius);
            
            // Color (based on position for variety)
            mesh.Vertices.push_back((x + 1.0f) * 0.5f);
            mesh.Vertices.push_back((y + 1.0f) * 0.5f);
            mesh.Vertices.push_back((z + 1.0f) * 0.5f);
        }
    }
    
    // Generate sphere indices
    for (int lat = 0; lat < segments; ++lat) {
        for (int lon = 0; lon < segments; ++lon) {
            int first = (lat * (segments + 1)) + lon;
            int second = first + segments + 1;
            
            mesh.Indices.push_back(first);
            mesh.Indices.push_back(second);
            mesh.Indices.push_back(first + 1);
            
            mesh.Indices.push_back(second);
            mesh.Indices.push_back(second + 1);
            mesh.Indices.push_back(first + 1);
        }
    }
    
    return mesh;
}

} // namespace MeshFactory
} // namespace Nilos

