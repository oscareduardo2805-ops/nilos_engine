#pragma once

#include "Collision.h"
#include "../ECS/Component.h"
#include "../ECS/Entity.h"
#include <vector>
#include <cstdint>

namespace Nilos {


/**
 * @brief Simple physics world - AABB collisions and gravity
 * 
 * Lightweight physics for NPCs and basic gameplay.
 * No rigid body dynamics - just collisions and movement.
 */
class PhysicsWorld {
public:
    PhysicsWorld() = default;
    ~PhysicsWorld() = default;

    /**
     * @brief Update physics (apply gravity, detect collisions)
     */
    void Update(float deltaTime);

    /**
     * @brief Register a rigidbody for physics simulation
     */
    void RegisterRigidbody(RigidbodyComponent* rb, ColliderComponent* collider, TransformComponent* transform);
    
    /**
     * @brief Register a static collider (no rigidbody, never moves)
     */
    void RegisterStaticCollider(ColliderComponent* collider, TransformComponent* transform);

    /**
     * @brief Check collision between two AABBs
     */
    bool CheckCollision(const AABB& a, const AABB& b) const;

    /**
     * @brief Raycast against all colliders
     * @return True if hit something, false otherwise
     */
    bool Raycast(const Ray& ray, float maxDistance, glm::vec3& hitPoint, Entity& hitEntity);

    /**
     * @brief Set global gravity
     */
    void SetGravity(const glm::vec3& gravity) { m_Gravity = gravity; }
    glm::vec3 GetGravity() const { return m_Gravity; }

    /**
     * @brief Clear all registered colliders
     */
    void Clear();

private:
    struct RigidbodyEntry {
        RigidbodyComponent* Rigidbody;
        ColliderComponent* Collider;
        TransformComponent* Transform;
        uint32_t EntityID;
    };
    
    struct StaticColliderEntry {
        ColliderComponent* Collider;
        TransformComponent* Transform;
        uint32_t EntityID;
    };

    std::vector<RigidbodyEntry> m_Rigidbodies;
    std::vector<StaticColliderEntry> m_StaticColliders;
    glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);

    /**
     * @brief Get world-space AABB for a collider
     */
    AABB GetWorldAABB(const ColliderComponent* collider, const TransformComponent* transform) const;
};

} // namespace Nilos

