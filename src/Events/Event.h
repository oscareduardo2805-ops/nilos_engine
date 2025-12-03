#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Nilos {

/**
 * @brief Base event class
 * 
 * All events should inherit from this class.
 * Events are used for communication between systems without tight coupling.
 */
class Event {
public:
    virtual ~Event() = default;
    virtual const char* GetName() const = 0;
    
    bool Handled = false;
};

// ============================================================================
// WINDOW EVENTS
// ============================================================================

/**
 * @brief Window close event
 */
class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;
    const char* GetName() const override { return "WindowCloseEvent"; }
};

/**
 * @brief Window resize event
 */
class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(uint32_t width, uint32_t height)
        : Width(width), Height(height) {}
    
    const char* GetName() const override { return "WindowResizeEvent"; }
    
    uint32_t Width;
    uint32_t Height;
};

// ============================================================================
// INPUT EVENTS
// ============================================================================

/**
 * @brief Key press event
 */
class KeyPressedEvent : public Event {
public:
    KeyPressedEvent(int keyCode, bool repeat)
        : KeyCode(keyCode), IsRepeat(repeat) {}
    
    const char* GetName() const override { return "KeyPressedEvent"; }
    
    int KeyCode;
    bool IsRepeat;
};

/**
 * @brief Key release event
 */
class KeyReleasedEvent : public Event {
public:
    explicit KeyReleasedEvent(int keyCode) : KeyCode(keyCode) {}
    
    const char* GetName() const override { return "KeyReleasedEvent"; }
    
    int KeyCode;
};

/**
 * @brief Mouse button press event
 */
class MouseButtonPressedEvent : public Event {
public:
    explicit MouseButtonPressedEvent(int button) : Button(button) {}
    
    const char* GetName() const override { return "MouseButtonPressedEvent"; }
    
    int Button;
};

/**
 * @brief Mouse button release event
 */
class MouseButtonReleasedEvent : public Event {
public:
    explicit MouseButtonReleasedEvent(int button) : Button(button) {}
    
    const char* GetName() const override { return "MouseButtonReleasedEvent"; }
    
    int Button;
};

/**
 * @brief Mouse move event
 */
class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float x, float y) : X(x), Y(y) {}
    
    const char* GetName() const override { return "MouseMovedEvent"; }
    
    float X;
    float Y;
};

/**
 * @brief Mouse scroll event
 */
class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : XOffset(xOffset), YOffset(yOffset) {}
    
    const char* GetName() const override { return "MouseScrolledEvent"; }
    
    float XOffset;
    float YOffset;
};

// ============================================================================
// PHYSICS EVENTS (Placeholder for Phase 3)
// ============================================================================

/**
 * @brief Collision event between two entities
 */
class CollisionEvent : public Event {
public:
    CollisionEvent(uint32_t entityA, uint32_t entityB, const glm::vec3& contactPoint)
        : EntityA(entityA), EntityB(entityB), ContactPoint(contactPoint) {}
    
    const char* GetName() const override { return "CollisionEvent"; }
    
    uint32_t EntityA;
    uint32_t EntityB;
    glm::vec3 ContactPoint;
};

/**
 * @brief Trigger event (when entity enters trigger collider)
 */
class TriggerEnterEvent : public Event {
public:
    TriggerEnterEvent(uint32_t trigger, uint32_t other)
        : TriggerEntity(trigger), OtherEntity(other) {}
    
    const char* GetName() const override { return "TriggerEnterEvent"; }
    
    uint32_t TriggerEntity;
    uint32_t OtherEntity;
};

// ============================================================================
// AI EVENTS (Placeholder for Phase 5)
// ============================================================================

/**
 * @brief AI perception event (when NPC detects something)
 */
class AIPerceptionEvent : public Event {
public:
    AIPerceptionEvent(uint32_t observer, uint32_t target, float distance)
        : ObserverEntity(observer), TargetEntity(target), Distance(distance) {}
    
    const char* GetName() const override { return "AIPerceptionEvent"; }
    
    uint32_t ObserverEntity;
    uint32_t TargetEntity;
    float Distance;
};

} // namespace Nilos

