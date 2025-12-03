# Entity Component System (ECS) Guide

This guide explains how to use the ECS architecture in Nilos Engine.

## What is ECS?

ECS (Entity Component System) is a design pattern that separates:
- **Data** (Components)
- **Logic** (Systems)
- **Identity** (Entities)

This provides flexibility, performance, and maintainability.

## Core Concepts

### Entities

Entities are just IDs. They represent "things" in your game world.

```cpp
// Create an entity
Entity player = world->CreateEntity("Player");

// Entity is just a number
// It holds no data, just identifies an object
```

### Components

Components are pure data structures. They hold state but no behavior.

```cpp
// Transform component holds position, rotation, scale
struct TransformComponent {
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

// Add component to entity
auto* transform = world->AddComponent<TransformComponent>(player);
transform->Position = glm::vec3(0, 0, 0);
```

### Systems

Systems are pure logic. They process entities with specific components.

```cpp
// Physics system operates on entities with Transform + Rigidbody
class PhysicsSystem : public System {
    void Update(float deltaTime) override {
        auto entities = world->GetEntitiesWithComponent<RigidbodyComponent>();
        for (auto entity : entities) {
            auto* rb = world->GetComponent<RigidbodyComponent>(entity);
            auto* transform = world->GetComponent<TransformComponent>(entity);
            
            // Apply physics
            rb->Velocity += gravity * deltaTime;
            transform->Position += rb->Velocity * deltaTime;
        }
    }
};
```

## Creating Entities

### Basic Entity

```cpp
// Create entity
Entity enemy = world->CreateEntity("Enemy");

// Add components
auto* transform = world->AddComponent<TransformComponent>(enemy);
transform->Position = glm::vec3(5, 0, 0);

auto* mesh = world->AddComponent<MeshComponent>(enemy);
mesh->CreateCube();
```

### Entity with Multiple Components

```cpp
Entity npc = world->CreateEntity("Villager");

// Transform
auto* transform = world->AddComponent<TransformComponent>(npc);
transform->Position = glm::vec3(10, 0, 5);

// Mesh (visual representation)
auto* mesh = world->AddComponent<MeshComponent>(npc);
mesh->CreateCube();

// Rigidbody (physics)
auto* rigidbody = world->AddComponent<RigidbodyComponent>(npc);
rigidbody->Mass = 70.0f;
rigidbody->UseGravity = true;

// Collider (collision detection)
auto* collider = world->AddComponent<ColliderComponent>(npc);
collider->ColliderType = ColliderComponent::Type::Capsule;
collider->Height = 1.8f;
collider->Radius = 0.3f;

// AI Agent
auto* ai = world->AddComponent<AIAgentComponent>(npc);
ai->PerceptionRadius = 10.0f;
```

## Querying Entities

### Get All Entities with a Component

```cpp
// Get all entities with mesh components
auto meshEntities = world->GetEntitiesWithComponent<MeshComponent>();

for (Entity entity : meshEntities) {
    auto* mesh = world->GetComponent<MeshComponent>(entity);
    auto* transform = world->GetComponent<TransformComponent>(entity);
    
    // Render mesh at transform position
    renderer->RenderMesh(*mesh, *transform);
}
```

### Check if Entity Has Component

```cpp
if (world->HasComponent<AIAgentComponent>(entity)) {
    auto* ai = world->GetComponent<AIAgentComponent>(entity);
    // Do AI stuff
}
```

### Get Component (Null if Missing)

```cpp
auto* transform = world->GetComponent<TransformComponent>(entity);
if (transform) {
    // Entity has transform component
    transform->Position.y += 1.0f;
} else {
    // Entity doesn't have transform
}
```

## Working with Components

### Modifying Components

```cpp
// Get component
auto* transform = world->GetComponent<TransformComponent>(entity);

// Modify data
transform->Position += glm::vec3(0, 1, 0);
transform->Rotation.y += 90.0f;
transform->Scale *= 1.1f;
```

### Removing Components

```cpp
// Remove a component from an entity
world->RemoveComponent<RigidbodyComponent>(entity);

// Entity still exists, just doesn't have physics anymore
```

### Adding Components Later

```cpp
// Entity starts simple
Entity obj = world->CreateEntity("Box");
auto* transform = world->AddComponent<TransformComponent>(obj);

// Later, make it physical
auto* rigidbody = world->AddComponent<RigidbodyComponent>(obj);
rigidbody->Mass = 10.0f;

// Now physics system will process it
```

## Creating Custom Components

### Define Component Structure

```cpp
// In Component.h or your own header

struct HealthComponent {
    float CurrentHealth = 100.0f;
    float MaxHealth = 100.0f;
    bool IsInvulnerable = false;
};

struct InventoryComponent {
    std::vector<uint32_t> ItemIds;
    int MaxSlots = 20;
};

struct DialogueComponent {
    std::string CurrentDialogue;
    std::vector<std::string> DialogueOptions;
    bool IsInConversation = false;
};
```

### Use Custom Components

```cpp
// Add to entity
Entity player = world->CreateEntity("Player");

auto* health = world->AddComponent<HealthComponent>(player);
health->CurrentHealth = 100.0f;
health->MaxHealth = 100.0f;

auto* inventory = world->AddComponent<InventoryComponent>(player);
inventory->MaxSlots = 30;

// Query entities with custom components
auto healthEntities = world->GetEntitiesWithComponent<HealthComponent>();
```

## Creating Custom Systems

### Define System Class

```cpp
// HealthSystem.h
#pragma once
#include "../ECS/System.h"
#include "../ECS/World.h"

class HealthSystem : public System {
public:
    void Initialize() override {
        NILOS_INFO("HealthSystem initialized");
    }

    void Update(float deltaTime) override {
        // Get all entities with health
        auto entities = m_World->GetEntitiesWithComponent<HealthComponent>();
        
        for (Entity entity : entities) {
            auto* health = m_World->GetComponent<HealthComponent>(entity);
            
            // Regenerate health over time
            if (health->CurrentHealth < health->MaxHealth) {
                health->CurrentHealth += 5.0f * deltaTime;
                if (health->CurrentHealth > health->MaxHealth) {
                    health->CurrentHealth = health->MaxHealth;
                }
            }
            
            // Check for death
            if (health->CurrentHealth <= 0.0f) {
                HandleDeath(entity);
            }
        }
    }

    void Shutdown() override {
        NILOS_INFO("HealthSystem shutdown");
    }

    const char* GetName() const override {
        return "HealthSystem";
    }

private:
    void HandleDeath(Entity entity) {
        // Dispatch death event
        EventManager::Get().Dispatch(EntityDeathEvent(entity));
        
        // Could destroy entity, disable it, play animation, etc.
    }
};
```

### Register System

```cpp
// In Engine.cpp or wherever you initialize systems

void Engine::Initialize() {
    // ... other initialization ...
    
    // Create ECS world
    m_World = std::make_unique<World>();
    m_World->Initialize();
    
    // Register systems
    m_World->RegisterSystem<PhysicsSystem>();
    m_World->RegisterSystem<HealthSystem>();  // Register custom system
    m_World->RegisterSystem<AISystem>();
    
    // Systems will be updated every frame automatically
}
```

## Common Patterns

### Player Entity

```cpp
Entity CreatePlayer(World* world, const glm::vec3& position) {
    Entity player = world->CreateEntity("Player");
    
    // Transform
    auto* transform = world->AddComponent<TransformComponent>(player);
    transform->Position = position;
    transform->Scale = glm::vec3(1.0f);
    
    // Visual
    auto* mesh = world->AddComponent<MeshComponent>(player);
    mesh->CreateCube();
    
    // Physics
    auto* rigidbody = world->AddComponent<RigidbodyComponent>(player);
    rigidbody->Mass = 75.0f;
    rigidbody->UseGravity = true;
    
    auto* collider = world->AddComponent<ColliderComponent>(player);
    collider->ColliderType = ColliderComponent::Type::Capsule;
    collider->Height = 1.8f;
    collider->Radius = 0.4f;
    
    // Gameplay
    auto* health = world->AddComponent<HealthComponent>(player);
    health->MaxHealth = 100.0f;
    health->CurrentHealth = 100.0f;
    
    auto* inventory = world->AddComponent<InventoryComponent>(player);
    inventory->MaxSlots = 30;
    
    return player;
}
```

### Static Environment Object

```cpp
Entity CreateStaticObject(World* world, const glm::vec3& position) {
    Entity obj = world->CreateEntity("StaticObject");
    
    // Only transform and mesh, no physics
    auto* transform = world->AddComponent<TransformComponent>(obj);
    transform->Position = position;
    
    auto* mesh = world->AddComponent<MeshComponent>(obj);
    mesh->CreateCube();
    
    // No rigidbody = won't move or participate in physics
    
    return obj;
}
```

### Particle Effect

```cpp
Entity CreateParticle(World* world, const glm::vec3& position, const glm::vec3& velocity) {
    Entity particle = world->CreateEntity("Particle");
    
    auto* transform = world->AddComponent<TransformComponent>(particle);
    transform->Position = position;
    transform->Scale = glm::vec3(0.1f);
    
    auto* mesh = world->AddComponent<MeshComponent>(particle);
    mesh->CreateCube();
    
    auto* rigidbody = world->AddComponent<RigidbodyComponent>(particle);
    rigidbody->Velocity = velocity;
    rigidbody->UseGravity = true;
    rigidbody->Mass = 0.1f;
    
    auto* lifetime = world->AddComponent<LifetimeComponent>(particle);
    lifetime->TimeToLive = 2.0f;  // Destroy after 2 seconds
    
    return particle;
}
```

## Best Practices

### 1. Keep Components Pure Data

❌ **Bad** - Component with logic:
```cpp
struct HealthComponent {
    float Health;
    
    void TakeDamage(float amount) {  // DON'T DO THIS
        Health -= amount;
    }
};
```

✅ **Good** - Pure data:
```cpp
struct HealthComponent {
    float CurrentHealth;
    float MaxHealth;
};

// Logic goes in system
class HealthSystem : public System {
    void ApplyDamage(Entity entity, float amount) {
        auto* health = m_World->GetComponent<HealthComponent>(entity);
        health->CurrentHealth -= amount;
    }
};
```

### 2. Use Small, Focused Components

❌ **Bad** - Monolithic component:
```cpp
struct CharacterComponent {
    glm::vec3 Position;
    float Health;
    std::vector<Item> Inventory;
    AI::BehaviorTree Brain;
    // ... 50 more fields
};
```

✅ **Good** - Focused components:
```cpp
struct TransformComponent { glm::vec3 Position; /* ... */ };
struct HealthComponent { float CurrentHealth; /* ... */ };
struct InventoryComponent { std::vector<Item> Items; };
struct AIComponent { /* AI data */ };
```

### 3. Query Only What You Need

❌ **Bad** - Unnecessary queries:
```cpp
void Update(float dt) {
    auto allEntities = world->GetAllEntities();  // Inefficient
    for (auto e : allEntities) {
        if (world->HasComponent<HealthComponent>(e)) {
            // Process health
        }
    }
}
```

✅ **Good** - Direct query:
```cpp
void Update(float dt) {
    auto healthEntities = world->GetEntitiesWithComponent<HealthComponent>();
    for (auto e : healthEntities) {
        auto* health = world->GetComponent<HealthComponent>(e);
        // Process health
    }
}
```

### 4. Use Events for Cross-System Communication

```cpp
// When entity dies in HealthSystem
EventManager::Get().Dispatch(EntityDeathEvent(entity));

// AudioSystem listens
EventManager::Get().Subscribe<EntityDeathEvent>([](const EntityDeathEvent& e) {
    PlayDeathSound(e.Entity);
});

// ParticleSystem also listens
EventManager::Get().Subscribe<EntityDeathEvent>([](const EntityDeathEvent& e) {
    SpawnDeathParticles(e.Entity);
});
```

## Advanced Topics

### Entity Prefabs (Future)

```cpp
// Define prefab
Prefab enemyPrefab;
enemyPrefab.AddComponent<TransformComponent>();
enemyPrefab.AddComponent<MeshComponent>();
enemyPrefab.AddComponent<HealthComponent>();

// Instantiate
Entity enemy1 = world->Instantiate(enemyPrefab, position1);
Entity enemy2 = world->Instantiate(enemyPrefab, position2);
```

### Entity Hierarchies (Future)

```cpp
// Parent-child relationships
Entity parent = world->CreateEntity("Parent");
Entity child = world->CreateEntity("Child");
world->SetParent(child, parent);

// Child transforms are relative to parent
```

### Component Dependencies

```cpp
// Some components require others
// Example: MeshComponent requires TransformComponent

if (world->HasComponent<MeshComponent>(entity)) {
    assert(world->HasComponent<TransformComponent>(entity));
}
```

## Summary

The ECS pattern provides:
- **Flexibility**: Mix and match components
- **Performance**: Cache-friendly memory layout
- **Clarity**: Separation of data and logic
- **Scalability**: Easy to extend with new components/systems

Start simple, add complexity as needed!

