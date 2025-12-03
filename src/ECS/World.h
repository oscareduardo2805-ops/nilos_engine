#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "../Core/Logger.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <any>
#include <algorithm>

namespace Nilos {

/**
 * @brief The World manages all entities, components, and systems
 * 
 * This is the core of the ECS architecture. It provides:
 * - Entity creation and destruction
 * - Component addition, removal, and access
 * - System registration and update
 * - Entity queries by component type
 * 
 * Usage example:
 *   auto world = std::make_unique<World>();
 *   Entity entity = world->CreateEntity("MyEntity");
 *   world->AddComponent<TransformComponent>(entity);
 *   world->RegisterSystem<PhysicsSystem>();
 */
class World {
public:
    World() : m_NextEntityId(1) {}
    ~World() = default;

    /**
     * @brief Initialize the world
     */
    void Initialize() {
        NILOS_INFO("World initialized");
    }

    /**
     * @brief Update all systems
     */
    void Update(float deltaTime) {
        for (auto& system : m_Systems) {
            if (system->IsEnabled()) {
                system->Update(deltaTime);
            }
        }
    }

    /**
     * @brief Shutdown the world and all systems
     */
    void Shutdown() {
        for (auto& system : m_Systems) {
            system->Shutdown();
        }
        m_Systems.clear();
        m_ComponentPools.clear();
        m_EntityNames.clear();
        NILOS_INFO("World shutdown");
    }

    // ========================================================================
    // Entity Management
    // ========================================================================

    /**
     * @brief Create a new entity
     * @param name Optional name for debugging
     * @return The entity ID
     */
    Entity CreateEntity(const std::string& name = "") {
        Entity entity = m_NextEntityId++;
        
        if (!name.empty()) {
            m_EntityNames[entity] = name;
        }
        
        return entity;
    }

    /**
     * @brief Destroy an entity and all its components
     */
    void DestroyEntity(Entity entity) {
        // Remove all components
        for (auto& [type, pool] : m_ComponentPools) {
            auto& componentMap = std::any_cast<std::unordered_map<Entity, std::any>&>(pool);
            componentMap.erase(entity);
        }
        
        // Remove name
        m_EntityNames.erase(entity);
    }

    /**
     * @brief Get entity name (if set)
     */
    std::string GetEntityName(Entity entity) const {
        auto it = m_EntityNames.find(entity);
        return (it != m_EntityNames.end()) ? it->second : "Entity_" + std::to_string(entity);
    }

    // ========================================================================
    // Component Management
    // ========================================================================

    /**
     * @brief Add a component to an entity
     * @return Pointer to the newly created component
     */
    template<typename T>
    T* AddComponent(Entity entity) {
        std::type_index typeIndex = typeid(T);
        
        // Get or create component pool
        if (m_ComponentPools.find(typeIndex) == m_ComponentPools.end()) {
            m_ComponentPools[typeIndex] = std::unordered_map<Entity, std::any>();
        }
        
        auto& pool = std::any_cast<std::unordered_map<Entity, std::any>&>(m_ComponentPools[typeIndex]);
        
        // Create and store component
        T component{};
        pool[entity] = component;
        
        return &std::any_cast<T&>(pool[entity]);
    }

    /**
     * @brief Remove a component from an entity
     */
    template<typename T>
    void RemoveComponent(Entity entity) {
        std::type_index typeIndex = typeid(T);
        
        auto poolIt = m_ComponentPools.find(typeIndex);
        if (poolIt != m_ComponentPools.end()) {
            auto& pool = std::any_cast<std::unordered_map<Entity, std::any>&>(poolIt->second);
            pool.erase(entity);
        }
    }

    /**
     * @brief Get a component from an entity
     * @return Pointer to component, or nullptr if not found
     */
    template<typename T>
    T* GetComponent(Entity entity) {
        std::type_index typeIndex = typeid(T);
        
        auto poolIt = m_ComponentPools.find(typeIndex);
        if (poolIt == m_ComponentPools.end()) {
            return nullptr;
        }
        
        auto& pool = std::any_cast<std::unordered_map<Entity, std::any>&>(poolIt->second);
        auto componentIt = pool.find(entity);
        
        if (componentIt == pool.end()) {
            return nullptr;
        }
        
        return &std::any_cast<T&>(componentIt->second);
    }

    /**
     * @brief Check if an entity has a component
     */
    template<typename T>
    bool HasComponent(Entity entity) const {
        std::type_index typeIndex = typeid(T);
        
        auto poolIt = m_ComponentPools.find(typeIndex);
        if (poolIt == m_ComponentPools.end()) {
            return false;
        }
        
        const auto& pool = std::any_cast<const std::unordered_map<Entity, std::any>&>(poolIt->second);
        return pool.find(entity) != pool.end();
    }

    /**
     * @brief Get all entities that have a specific component
     */
    template<typename T>
    std::vector<Entity> GetEntitiesWithComponent() {
        std::vector<Entity> entities;
        std::type_index typeIndex = typeid(T);
        
        auto poolIt = m_ComponentPools.find(typeIndex);
        if (poolIt != m_ComponentPools.end()) {
            const auto& pool = std::any_cast<const std::unordered_map<Entity, std::any>&>(poolIt->second);
            entities.reserve(pool.size());
            
            for (const auto& [entity, component] : pool) {
                entities.push_back(entity);
            }
        }
        
        return entities;
    }

    // ========================================================================
    // System Management
    // ========================================================================

    /**
     * @brief Register a new system
     */
    template<typename T>
    T* RegisterSystem() {
        auto system = std::make_unique<T>();
        T* systemPtr = system.get();
        
        system->Initialize();
        m_Systems.push_back(std::move(system));
        
        NILOS_INFO("System registered: ", systemPtr->GetName());
        return systemPtr;
    }

    /**
     * @brief Get a registered system by type
     */
    template<typename T>
    T* GetSystem() {
        for (auto& system : m_Systems) {
            T* castedSystem = dynamic_cast<T*>(system.get());
            if (castedSystem) {
                return castedSystem;
            }
        }
        return nullptr;
    }

private:
    Entity m_NextEntityId;
    
    // Component storage: type -> (entity -> component)
    std::unordered_map<std::type_index, std::any> m_ComponentPools;
    
    // Entity names for debugging
    std::unordered_map<Entity, std::string> m_EntityNames;
    
    // Registered systems
    std::vector<std::unique_ptr<System>> m_Systems;
};

} // namespace Nilos

