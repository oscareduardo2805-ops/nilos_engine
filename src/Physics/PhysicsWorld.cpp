#include "PhysicsWorld.h"
#include "../Core/Logger.h"
#include <algorithm>

namespace Nilos {

void PhysicsWorld::Update(float deltaTime) {
    // Step 1: Apply forces (gravity, etc.)
    for (auto& entry : m_Rigidbodies) {
        RigidbodyComponent* rb = entry.Rigidbody;
        
        if (!rb || rb->IsStatic || rb->IsKinematic) continue;
        
        // Apply gravity
        if (rb->UseGravity) {
            rb->AddForce(m_Gravity * rb->Mass);
        }
        
        // Apply accumulated forces: F = ma -> a = F/m
        glm::vec3 acceleration = rb->Force * rb->InverseMass;
        rb->Velocity += acceleration * deltaTime;
        
        // Apply linear damping (air resistance)
        rb->Velocity *= (1.0f - rb->LinearDamping);
        
        // Clear forces for next frame
        rb->ClearForces();
    }
    
    // Step 2: Integrate velocity -> position
    for (auto& entry : m_Rigidbodies) {
        RigidbodyComponent* rb = entry.Rigidbody;
        TransformComponent* transform = entry.Transform;
        ColliderComponent* collider = entry.Collider;
        
        if (!rb || !transform || rb->IsStatic) continue;
        
        // Update position
        transform->Position += rb->Velocity * deltaTime;
        
        // Update rotation (angular velocity)
        if (glm::length(rb->AngularVelocity) > 0.0f) {
            glm::vec3 axis = glm::normalize(rb->AngularVelocity);
            float angle = glm::length(rb->AngularVelocity) * deltaTime;
            // Simple angular integration (Euler)
            transform->Rotation += rb->AngularVelocity * deltaTime;
            rb->AngularVelocity *= (1.0f - rb->AngularDamping);
        }
    }
    
    // Step 3: Ground collision (simple for now)
    for (auto& entry : m_Rigidbodies) {
        RigidbodyComponent* rb = entry.Rigidbody;
        TransformComponent* transform = entry.Transform;
        ColliderComponent* collider = entry.Collider;
        
        if (!rb || !transform || !collider || rb->IsStatic) continue;
        
        // Calculate bottom of object
        float objectBottom = transform->Position.y - (collider->Size.y * transform->Scale.y * 0.5f);
        
        // Ground is at Y = 0
        if (objectBottom <= 0.0f) {
            // Place on ground
            transform->Position.y = (collider->Size.y * transform->Scale.y * 0.5f);
            
            // Bounce
            if (rb->Velocity.y < 0.0f) {
                rb->Velocity.y = -rb->Velocity.y * rb->Restitution;
                
                // Apply friction to horizontal velocity
                rb->Velocity.x *= (1.0f - rb->DynamicFriction);
                rb->Velocity.z *= (1.0f - rb->DynamicFriction);
                
                // Stop bouncing if too slow
                if (glm::abs(rb->Velocity.y) < 0.05f) {
                    rb->Velocity.y = 0.0f;
                    
                    // Apply static friction
                    if (glm::length(glm::vec2(rb->Velocity.x, rb->Velocity.z)) < 0.1f) {
                        rb->Velocity = glm::vec3(0.0f);
                    }
                }
            }
        }
    }
    
    // Step 4: Object-object collisions (AABB)
    for (size_t i = 0; i < m_Rigidbodies.size(); ++i) {
        RigidbodyComponent* rbA = m_Rigidbodies[i].Rigidbody;
        ColliderComponent* colA = m_Rigidbodies[i].Collider;
        TransformComponent* transA = m_Rigidbodies[i].Transform;
        
        if (!rbA || !colA || !transA || rbA->IsStatic) continue;
        
        AABB aabbA = GetWorldAABB(colA, transA);
        
        for (size_t j = i + 1; j < m_Rigidbodies.size(); ++j) {
            RigidbodyComponent* rbB = m_Rigidbodies[j].Rigidbody;
            ColliderComponent* colB = m_Rigidbodies[j].Collider;
            TransformComponent* transB = m_Rigidbodies[j].Transform;
            
            if (!rbB || !colB || !transB) continue;
            
            AABB aabbB = GetWorldAABB(colB, transB);
            
            if (aabbA.Intersects(aabbB)) {
                // Simple collision response: push apart
                glm::vec3 centerA = aabbA.GetCenter();
                glm::vec3 centerB = aabbB.GetCenter();
                glm::vec3 normal = glm::normalize(centerA - centerB);
                
                if (glm::length(normal) < 0.001f) {
                    normal = glm::vec3(0.0f, 1.0f, 0.0f);
                }
                
                // Push objects apart
                if (!rbA->IsStatic) {
                    transA->Position += normal * 0.01f;
                }
                if (!rbB->IsStatic) {
                    transB->Position -= normal * 0.01f;
                }
                
                // Simple impulse (bounce)
                float restitution = (rbA->Restitution + rbB->Restitution) * 0.5f;
                glm::vec3 relativeVelocity = rbA->Velocity - rbB->Velocity;
                float velocityAlongNormal = glm::dot(relativeVelocity, normal);
                
                if (velocityAlongNormal > 0) continue; // Moving apart
                
                float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
                impulseScalar /= (rbA->InverseMass + rbB->InverseMass);
                
                glm::vec3 impulse = impulseScalar * normal;
                
                if (!rbA->IsStatic) {
                    rbA->Velocity += impulse * rbA->InverseMass;
                }
                if (!rbB->IsStatic) {
                    rbB->Velocity -= impulse * rbB->InverseMass;
                }
            }
        }
    }
}

void PhysicsWorld::RegisterRigidbody(RigidbodyComponent* rb, ColliderComponent* collider, TransformComponent* transform) {
    RigidbodyEntry entry;
    entry.Rigidbody = rb;
    entry.Collider = collider;
    entry.Transform = transform;
    entry.EntityID = 0;
    m_Rigidbodies.push_back(entry);
}

void PhysicsWorld::RegisterStaticCollider(ColliderComponent* collider, TransformComponent* transform) {
    StaticColliderEntry entry;
    entry.Collider = collider;
    entry.Transform = transform;
    entry.EntityID = 0;
    m_StaticColliders.push_back(entry);
}

bool PhysicsWorld::CheckCollision(const AABB& a, const AABB& b) const {
    return a.Intersects(b);
}

bool PhysicsWorld::Raycast(const Ray& ray, float maxDistance, glm::vec3& hitPoint, Entity& hitEntity) {
    float closestT = maxDistance;
    bool hit = false;
    
    // Check rigidbodies
    for (const auto& entry : m_Rigidbodies) {
        AABB aabb = GetWorldAABB(entry.Collider, entry.Transform);
        
        float tMin, tMax;
        if (ray.Intersects(aabb, tMin, tMax)) {
            if (tMin < closestT && tMin >= 0.0f) {
                closestT = tMin;
                hitPoint = ray.GetPoint(tMin);
                hitEntity = entry.EntityID;
                hit = true;
            }
        }
    }
    
    // Check static colliders
    for (const auto& entry : m_StaticColliders) {
        AABB aabb = GetWorldAABB(entry.Collider, entry.Transform);
        
        float tMin, tMax;
        if (ray.Intersects(aabb, tMin, tMax)) {
            if (tMin < closestT && tMin >= 0.0f) {
                closestT = tMin;
                hitPoint = ray.GetPoint(tMin);
                hitEntity = entry.EntityID;
                hit = true;
            }
        }
    }
    
    return hit;
}

void PhysicsWorld::Clear() {
    m_Rigidbodies.clear();
    m_StaticColliders.clear();
}

AABB PhysicsWorld::GetWorldAABB(const ColliderComponent* collider, const TransformComponent* transform) const {
    // Transform local AABB to world space
    glm::vec3 worldCenter = transform->Position + collider->Center * transform->Scale;
    glm::vec3 worldSize = collider->Size * transform->Scale;
    return AABB::FromCenterSize(worldCenter, worldSize);
}

} // namespace Nilos
