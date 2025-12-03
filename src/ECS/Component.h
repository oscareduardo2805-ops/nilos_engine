#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <cstdint>
#include "../Physics/Collision.h"

namespace Nilos {

/**
 * @brief Component type ID generator
 * 
 * Uses template specialization to generate unique IDs for each component type
 */
class ComponentTypeIdGenerator {
public:
    template<typename T>
    static uint32_t GetId() {
        static uint32_t id = s_NextId++;
        return id;
    }

private:
    static inline uint32_t s_NextId = 0;
};

// ============================================================================
// CORE COMPONENTS
// ============================================================================

/**
 * @brief Transform component - position, rotation, scale
 * 
 * Every spatial entity should have this component.
 * Rotation is stored in Euler angles (degrees).
 */
struct TransformComponent {
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f); // Euler angles in degrees
    glm::vec3 Scale = glm::vec3(1.0f);

    /**
     * @brief Calculate the model matrix from transform data
     */
    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        
        // Apply transformations: Translation * Rotation * Scale
        model = glm::translate(model, Position);
        model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, Scale);
        
        return model;
    }
};

/**
 * @brief Mesh component - renderable geometry data
 * 
 * Contains vertex data and GPU buffer IDs for rendering.
 * Vertex format: Position(3) + Normal(3) + Color(3) + TexCoord(2) = 11 floats
 */
struct MeshComponent {
    std::vector<float> Vertices;  // Interleaved vertex data
    std::vector<uint32_t> Indices;
    uint32_t VAO = 0; // Vertex Array Object
    uint32_t VBO = 0; // Vertex Buffer Object
    uint32_t EBO = 0; // Element Buffer Object
    glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f);
    uint32_t MaterialID = 0; // Optional material reference

    /**
     * @brief Create a simple cube mesh with normals (corrected geometry)
     */
    void CreateCube() {
        // Cube with Position(3) + Normal(3) + Color(3) + TexCoord(2) = 11 floats per vertex
        // All vertices use same color for solid appearance
        Vertices = {
            // Front face (+Z) - RED
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.8f, 0.3f, 0.3f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.8f, 0.3f, 0.3f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.8f, 0.3f, 0.3f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.8f, 0.3f, 0.3f,  0.0f, 1.0f,
            
            // Back face (-Z) - GREEN
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.3f, 0.8f, 0.3f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.3f, 0.8f, 0.3f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.3f, 0.8f, 0.3f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.3f, 0.8f, 0.3f,  0.0f, 1.0f,
            
            // Top face (+Y) - BLUE
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.3f, 0.3f, 0.8f,  0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.3f, 0.3f, 0.8f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.3f, 0.3f, 0.8f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.3f, 0.3f, 0.8f,  0.0f, 1.0f,
            
            // Bottom face (-Y) - YELLOW
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.8f, 0.8f, 0.3f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.8f, 0.8f, 0.3f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.8f, 0.8f, 0.3f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.8f, 0.8f, 0.3f,  0.0f, 1.0f,
            
            // Right face (+X) - MAGENTA
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.8f, 0.3f, 0.8f,  0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.8f, 0.3f, 0.8f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.8f, 0.3f, 0.8f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.8f, 0.3f, 0.8f,  0.0f, 1.0f,
            
            // Left face (-X) - CYAN
            -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.3f, 0.8f, 0.8f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.3f, 0.8f, 0.8f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.3f, 0.8f, 0.8f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.3f, 0.8f, 0.8f,  0.0f, 1.0f,
        };

        Indices = {
            0,  1,  2,   2,  3,  0,   // Front (CCW from outside)
            4,  5,  6,   6,  7,  4,   // Back
            8,  9, 10,  10, 11,  8,   // Top
            12, 13, 14,  14, 15, 12,  // Bottom
            16, 17, 18,  18, 19, 16,  // Right
            20, 21, 22,  22, 23, 20   // Left
        };
    }

    /**
     * @brief Create a sphere mesh (UV sphere)
     * @param radius Sphere radius
     * @param segments Horizontal segments (longitude)
     * @param rings Vertical rings (latitude)
     */
    void CreateSphere(float radius = 0.5f, int segments = 32, int rings = 16) {
        Vertices.clear();
        Indices.clear();
        
        // Generate vertices
        for (int ring = 0; ring <= rings; ++ring) {
            float v = (float)ring / (float)rings;
            float phi = v * glm::pi<float>();
            
            for (int seg = 0; seg <= segments; ++seg) {
                float u = (float)seg / (float)segments;
                float theta = u * 2.0f * glm::pi<float>();
                
                // Position
                float x = radius * sin(phi) * cos(theta);
                float y = radius * cos(phi);
                float z = radius * sin(phi) * sin(theta);
                
                // Normal (normalized position for sphere)
                float nx = sin(phi) * cos(theta);
                float ny = cos(phi);
                float nz = sin(phi) * sin(theta);
                
                // Color (orange for basketball)
                float r = 0.9f, g = 0.5f, b = 0.2f;
                
                // UV coordinates
                float tu = u;
                float tv = v;
                
                // Add vertex: Position(3) + Normal(3) + Color(3) + TexCoord(2)
                Vertices.push_back(x); Vertices.push_back(y); Vertices.push_back(z);
                Vertices.push_back(nx); Vertices.push_back(ny); Vertices.push_back(nz);
                Vertices.push_back(r); Vertices.push_back(g); Vertices.push_back(b);
                Vertices.push_back(tu); Vertices.push_back(tv);
            }
        }
        
        // Generate indices
        for (int ring = 0; ring < rings; ++ring) {
            for (int seg = 0; seg < segments; ++seg) {
                int current = ring * (segments + 1) + seg;
                int next = current + segments + 1;
                
                // Two triangles per quad
                Indices.push_back(current);
                Indices.push_back(next);
                Indices.push_back(current + 1);
                
                Indices.push_back(current + 1);
                Indices.push_back(next);
                Indices.push_back(next + 1);
            }
        }
    }

    /**
     * @brief Check if mesh has been uploaded to GPU
     */
    bool IsInitialized() const {
        return VAO != 0;
    }
};

/**
 * @brief Camera component - view and projection matrices
 */
struct CameraComponent {
    float FOV = 60.0f;        // Field of view in degrees
    float Near = 0.1f;        // Near clipping plane
    float Far = 100.0f;       // Far clipping plane
    float Yaw = -90.0f;       // Horizontal rotation
    float Pitch = 0.0f;       // Vertical rotation

    // Cached direction vectors
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);

    // Matrices
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);

    /**
     * @brief Update camera direction vectors based on yaw and pitch
     */
    void UpdateVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    /**
     * @brief Update projection matrix
     */
    void UpdateProjectionMatrix(float aspectRatio) {
        ProjectionMatrix = glm::perspective(
            glm::radians(FOV), 
            aspectRatio, 
            Near, 
            Far
        );
    }

    /**
     * @brief Get view matrix from position
     */
    glm::mat4 GetViewMatrix(const glm::vec3& position) const {
        return glm::lookAt(position, position + Front, Up);
    }

    glm::vec3 GetForward() const { return Front; }
    glm::vec3 GetRight() const { return Right; }
    glm::vec3 GetUp() const { return Up; }
};

// ============================================================================
// PHYSICS COMPONENTS (Placeholder for Phase 3)
// ============================================================================

/**
 * @brief Rigidbody component - realistic physics simulation
 * 
 * PRIMARY physics component for all physical objects.
 * Simulates real-world physics: gravity, forces, collisions, friction.
 */
struct RigidbodyComponent {
    // Mass properties
    float Mass = 1.0f;                    // kg
    float InverseMass = 1.0f;             // 1/Mass (computed)
    
    // Linear motion
    glm::vec3 Velocity = glm::vec3(0.0f); // m/s
    glm::vec3 Force = glm::vec3(0.0f);    // Accumulated forces
    
    // Angular motion
    glm::vec3 AngularVelocity = glm::vec3(0.0f); // rad/s
    glm::vec3 Torque = glm::vec3(0.0f);          // Accumulated torques
    
    // Material properties
    float Restitution = 0.5f;             // Bounciness (0-1)
    float StaticFriction = 0.6f;          // Friction when not moving
    float DynamicFriction = 0.4f;         // Friction when moving
    
    // Damping (air resistance)
    float LinearDamping = 0.01f;          // Velocity decay
    float AngularDamping = 0.05f;         // Angular velocity decay
    
    // Flags
    bool UseGravity = true;               // Affected by gravity?
    bool IsKinematic = false;             // Moves but ignores forces (for platforms, etc.)
    bool IsStatic = false;                // Never moves (for walls, ground)
    
    RigidbodyComponent() {
        UpdateInverseMass();
    }
    
    void SetMass(float mass) {
        Mass = mass;
        UpdateInverseMass();
    }
    
    void UpdateInverseMass() {
        InverseMass = (Mass > 0.0f) ? (1.0f / Mass) : 0.0f;
    }
    
    void AddForce(const glm::vec3& force) {
        if (!IsKinematic && !IsStatic) {
            Force += force;
        }
    }
    
    void ClearForces() {
        Force = glm::vec3(0.0f);
        Torque = glm::vec3(0.0f);
    }
};

/**
 * @brief Collider component for collision detection
 */
struct ColliderComponent {
    enum class Type {
        Box,
        Sphere,
        Capsule,
        Mesh
    };

    Type ColliderType = Type::Box;
    glm::vec3 Center = glm::vec3(0.0f);
    glm::vec3 Size = glm::vec3(1.0f);    // For box
    float Radius = 0.5f;                  // For sphere/capsule
    float Height = 2.0f;                  // For capsule
    bool IsTrigger = false;               // Trigger vs solid collider
    
    // Physics properties (Phase 3)
    glm::vec3 Velocity = glm::vec3(0.0f); // Linear velocity
    bool IsDynamic = false;               // Affected by physics?
    float Restitution = 0.75f;            // Bounce coefficient (0-1)
};

// ============================================================================
// AI COMPONENTS (Placeholder for Phase 5)
// ============================================================================

/**
 * @brief AI Agent component for NPC behavior
 */
struct AIAgentComponent {
    bool IsActive = true;
    float PerceptionRadius = 10.0f;
    float UpdateInterval = 0.1f;          // How often to update AI (seconds)
    float TimeSinceLastUpdate = 0.0f;
    
    // Placeholder for future AI state
    // Will include: behavior tree, GOAP planner, memory system, etc.
};

/**
 * @brief Name component for entity identification
 */
struct NameComponent {
    std::string Name;
    
    NameComponent() = default;
    explicit NameComponent(const std::string& name) : Name(name) {}
};

} // namespace Nilos

