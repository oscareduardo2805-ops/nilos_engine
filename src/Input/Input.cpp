#include "Input.h"
#include "../Core/Logger.h"

#include <GLFW/glfw3.h>

namespace Nilos {

Input::Input()
    : m_Window(nullptr)
    , m_MousePosition(0.0f)
    , m_PreviousMousePosition(0.0f)
    , m_MouseDelta(0.0f)
    , m_MouseScroll(0.0f)
    , m_FirstMouse(true)
{
    m_KeyStates.fill(false);
    m_PreviousKeyStates.fill(false);
    m_MouseButtonStates.fill(false);
    m_PreviousMouseButtonStates.fill(false);
}

void Input::Initialize(GLFWwindow* window) {
    m_Window = window;
    NILOS_DEBUG("Input system initialized");
}

void Input::Update() {
    if (!m_Window) return;

    // Update previous states
    m_PreviousKeyStates = m_KeyStates;
    m_PreviousMouseButtonStates = m_MouseButtonStates;
    m_PreviousMousePosition = m_MousePosition;

    // Query current key states (only valid GLFW keys: 32-348)
    for (size_t i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i) {
        if (i < MAX_KEYS) {
            m_KeyStates[i] = glfwGetKey(m_Window, static_cast<int>(i)) == GLFW_PRESS;
        }
    }

    // Query current mouse button states
    for (size_t i = 0; i < MAX_MOUSE_BUTTONS; ++i) {
        m_MouseButtonStates[i] = glfwGetMouseButton(m_Window, static_cast<int>(i)) == GLFW_PRESS;
    }

    // Query mouse position
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    m_MousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));

    // Calculate mouse delta
    if (m_FirstMouse) {
        m_PreviousMousePosition = m_MousePosition;
        m_FirstMouse = false;
    }
    m_MouseDelta = m_MousePosition - m_PreviousMousePosition;

    // Reset scroll (will be updated by callback if there's scrolling)
    m_MouseScroll = glm::vec2(0.0f);
}

bool Input::IsKeyDown(int keyCode) const {
    if (keyCode < 0 || keyCode >= static_cast<int>(MAX_KEYS)) {
        return false;
    }
    return m_KeyStates[keyCode];
}

bool Input::IsKeyPressed(int keyCode) const {
    if (keyCode < 0 || keyCode >= static_cast<int>(MAX_KEYS)) {
        return false;
    }
    return m_KeyStates[keyCode] && !m_PreviousKeyStates[keyCode];
}

bool Input::IsKeyReleased(int keyCode) const {
    if (keyCode < 0 || keyCode >= static_cast<int>(MAX_KEYS)) {
        return false;
    }
    return !m_KeyStates[keyCode] && m_PreviousKeyStates[keyCode];
}

bool Input::IsMouseButtonDown(int button) const {
    if (button < 0 || button >= static_cast<int>(MAX_MOUSE_BUTTONS)) {
        return false;
    }
    return m_MouseButtonStates[button];
}

bool Input::IsMouseButtonPressed(int button) const {
    if (button < 0 || button >= static_cast<int>(MAX_MOUSE_BUTTONS)) {
        return false;
    }
    return m_MouseButtonStates[button] && !m_PreviousMouseButtonStates[button];
}

bool Input::IsMouseButtonReleased(int button) const {
    if (button < 0 || button >= static_cast<int>(MAX_MOUSE_BUTTONS)) {
        return false;
    }
    return !m_MouseButtonStates[button] && m_PreviousMouseButtonStates[button];
}

} // namespace Nilos

