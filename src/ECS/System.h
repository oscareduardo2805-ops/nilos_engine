#pragma once

#include <string>

namespace Nilos {

/**
 * @brief Base class for all ECS systems
 * 
 * Systems contain the logic that operates on entities with specific components.
 * Each system should focus on a single responsibility (rendering, physics, AI, etc.)
 * 
 * Example: PhysicsSystem operates on entities with TransformComponent and RigidbodyComponent
 */
class System {
public:
    virtual ~System() = default;

    /**
     * @brief Initialize the system
     * 
     * Called once when the system is first created.
     * Use this for resource allocation, loading data, etc.
     */
    virtual void Initialize() {}

    /**
     * @brief Update the system
     * 
     * Called every frame. This is where the main logic happens.
     * @param deltaTime Time elapsed since last frame (in seconds)
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Shutdown the system
     * 
     * Called when the system is being destroyed.
     * Use this for cleanup and resource deallocation.
     */
    virtual void Shutdown() {}

    /**
     * @brief Get the name of this system (for debugging)
     */
    virtual const char* GetName() const = 0;

    /**
     * @brief Enable/disable the system
     */
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

protected:
    bool m_Enabled = true;
};

} // namespace Nilos

