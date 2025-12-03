#pragma once

#include "Collision.h"
#include "../ECS/Component.h"
#include "../ECS/Entity.h"
#include <vector>

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
     * @brief Register a collider component
     */
    void RegisterCollider(ColliderComponent* collider, TransformComponent* transform);

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
    struct ColliderEntry {
        ColliderComponent* Collider;
        TransformComponent* Transform;
        Entity EntityID;
    };

    std::vector<ColliderEntry> m_Colliders;
    glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f);

    /**
     * @brief Get world-space AABB for a collider
     */
    AABB GetWorldAABB(const ColliderComponent* collider, const TransformComponent* transform) const;
};

} // namespace Nilos

