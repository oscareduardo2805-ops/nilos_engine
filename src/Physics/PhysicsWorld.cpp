#include "PhysicsWorld.h"
#include "../Core/Logger.h"

namespace Nilos {

void PhysicsWorld::Update(float deltaTime) {
    // Simple gravity and basic physics
    // Phase 3: Just placeholder for now
    NILOS_DEBUG("Physics update (placeholder)");
}

void PhysicsWorld::RegisterCollider(ColliderComponent* collider, TransformComponent* transform) {
    // Placeholder
}

bool PhysicsWorld::CheckCollision(const AABB& a, const AABB& b) const {
    return a.Intersects(b);
}

bool PhysicsWorld::Raycast(const Ray& ray, float maxDistance, glm::vec3& hitPoint, Entity& hitEntity) {
    // Placeholder
    return false;
}

void PhysicsWorld::Clear() {
    m_Colliders.clear();
}

AABB PhysicsWorld::GetWorldAABB(const ColliderComponent* collider, const TransformComponent* transform) const {
    // Convert collider to AABB
    glm::vec3 worldCenter = transform->Position + collider->Center;
    return AABB::FromCenterSize(worldCenter, collider->Size * transform->Scale);
}

} // namespace Nilos
