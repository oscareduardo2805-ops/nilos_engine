#pragma once

#include "Event.h"
#include "../Core/Logger.h"

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>

namespace Nilos {

/**
 * @brief Event manager for decoupled communication between systems
 * 
 * Systems can subscribe to events they care about and dispatch events
 * when something interesting happens. This maintains loose coupling.
 * 
 * Usage:
 *   // Subscribe to an event
 *   EventManager::Get().Subscribe<CollisionEvent>([](const CollisionEvent& e) {
 *       // Handle collision
 *   });
 * 
 *   // Dispatch an event
 *   EventManager::Get().Dispatch(CollisionEvent(entityA, entityB));
 */
class EventManager {
public:
    using EventCallback = std::function<void(const Event&)>;

    /**
     * @brief Get the singleton instance
     */
    static EventManager& Get() {
        static EventManager instance;
        return instance;
    }

    /**
     * @brief Initialize the event system
     */
    void Initialize() {
        NILOS_DEBUG("EventManager initialized");
    }

    /**
     * @brief Shutdown the event system
     */
    void Shutdown() {
        m_Subscribers.clear();
        NILOS_DEBUG("EventManager shutdown");
    }

    /**
     * @brief Subscribe to an event type
     * @return Subscription ID (can be used to unsubscribe)
     */
    template<typename T>
    uint32_t Subscribe(std::function<void(const T&)> callback) {
        std::type_index typeIndex = typeid(T);
        
        // Wrap the typed callback in a generic one
        EventCallback genericCallback = [callback](const Event& event) {
            callback(static_cast<const T&>(event));
        };
        
        uint32_t subscriptionId = m_NextSubscriptionId++;
        m_Subscribers[typeIndex].push_back({subscriptionId, genericCallback});
        
        return subscriptionId;
    }

    /**
     * @brief Unsubscribe from an event
     */
    void Unsubscribe(uint32_t subscriptionId) {
        for (auto& [type, callbacks] : m_Subscribers) {
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [subscriptionId](const Subscription& sub) {
                        return sub.Id == subscriptionId;
                    }),
                callbacks.end()
            );
        }
    }

    /**
     * @brief Dispatch an event immediately
     * 
     * All subscribers will be notified synchronously.
     */
    template<typename T>
    void Dispatch(const T& event) {
        std::type_index typeIndex = typeid(T);
        
        auto it = m_Subscribers.find(typeIndex);
        if (it != m_Subscribers.end()) {
            for (auto& subscription : it->second) {
                subscription.Callback(event);
                
                // Stop propagation if event is marked as handled
                if (event.Handled) {
                    break;
                }
            }
        }
    }

    /**
     * @brief Queue an event for processing later
     * 
     * Events will be dispatched at the beginning of the next frame.
     */
    template<typename T>
    void QueueEvent(const T& event) {
        m_EventQueue.push_back(std::make_unique<T>(event));
    }

    /**
     * @brief Process all queued events
     * 
     * Should be called once per frame, usually at the beginning.
     */
    void ProcessQueue() {
        for (auto& event : m_EventQueue) {
            // Find the event type and dispatch
            // Note: This is a simplified version. Production code would need
            // a more sophisticated type-erased dispatch mechanism.
            // For now, we'll skip this in the demo.
        }
        m_EventQueue.clear();
    }

private:
    EventManager() : m_NextSubscriptionId(1) {}

    struct Subscription {
        uint32_t Id;
        EventCallback Callback;
    };

    uint32_t m_NextSubscriptionId;
    std::unordered_map<std::type_index, std::vector<Subscription>> m_Subscribers;
    std::vector<std::unique_ptr<Event>> m_EventQueue;
};

} // namespace Nilos

