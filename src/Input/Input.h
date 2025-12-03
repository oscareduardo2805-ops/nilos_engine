#pragma once

#include <glm/glm.hpp>
#include <array>

struct GLFWwindow;

namespace Nilos {

/**
 * @brief Input management system
 * 
 * Provides a clean interface to query keyboard and mouse state.
 * Handles key/button state tracking and mouse position.
 */
class Input {
public:
    /**
     * @brief Get the singleton instance
     */
    static Input& Get() {
        static Input instance;
        return instance;
    }

    /**
     * @brief Initialize the input system with a window
     */
    void Initialize(GLFWwindow* window);

    /**
     * @brief Update input state (call at beginning of frame)
     */
    void Update();

    // ========================================================================
    // Keyboard Input
    // ========================================================================

    /**
     * @brief Check if a key is currently pressed
     */
    bool IsKeyDown(int keyCode) const;

    /**
     * @brief Check if a key was just pressed this frame
     */
    bool IsKeyPressed(int keyCode) const;

    /**
     * @brief Check if a key was just released this frame
     */
    bool IsKeyReleased(int keyCode) const;

    // ========================================================================
    // Mouse Input
    // ========================================================================

    /**
     * @brief Check if a mouse button is currently pressed
     */
    bool IsMouseButtonDown(int button) const;

    /**
     * @brief Check if a mouse button was just pressed this frame
     */
    bool IsMouseButtonPressed(int button) const;

    /**
     * @brief Check if a mouse button was just released this frame
     */
    bool IsMouseButtonReleased(int button) const;

    /**
     * @brief Get current mouse position
     */
    glm::vec2 GetMousePosition() const { return m_MousePosition; }

    /**
     * @brief Get mouse delta (movement since last frame)
     */
    glm::vec2 GetMouseDelta() const { return m_MouseDelta; }

    /**
     * @brief Get mouse scroll delta
     */
    glm::vec2 GetMouseScroll() const { return m_MouseScroll; }

private:
    Input();

    GLFWwindow* m_Window;

    // Key states (max 350 keys for GLFW)
    static constexpr size_t MAX_KEYS = 350;
    std::array<bool, MAX_KEYS> m_KeyStates;
    std::array<bool, MAX_KEYS> m_PreviousKeyStates;

    // Mouse button states
    static constexpr size_t MAX_MOUSE_BUTTONS = 8;
    std::array<bool, MAX_MOUSE_BUTTONS> m_MouseButtonStates;
    std::array<bool, MAX_MOUSE_BUTTONS> m_PreviousMouseButtonStates;

    // Mouse position and delta
    glm::vec2 m_MousePosition;
    glm::vec2 m_PreviousMousePosition;
    glm::vec2 m_MouseDelta;
    glm::vec2 m_MouseScroll;
    bool m_FirstMouse;
};

} // namespace Nilos

