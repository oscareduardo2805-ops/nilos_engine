#include "Window.h"
#include "../Core/Logger.h"
#include "../Events/EventManager.h"
#include "../Events/Event.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nilos {

// Static callbacks for GLFW (can't use member functions directly)
static void GLFWErrorCallback(int error, const char* description) {
    NILOS_ERROR("GLFW Error (", error, "): ", description);
}

static void GLFWWindowCloseCallback(GLFWwindow* window) {
    EventManager::Get().Dispatch(WindowCloseEvent());
}

static void GLFWWindowSizeCallback(GLFWwindow* window, int width, int height) {
    EventManager::Get().Dispatch(WindowResizeEvent(width, height));
    
    // Update OpenGL viewport
    glViewport(0, 0, width, height);
}

static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        EventManager::Get().Dispatch(KeyPressedEvent(key, false));
    } else if (action == GLFW_RELEASE) {
        EventManager::Get().Dispatch(KeyReleasedEvent(key));
    } else if (action == GLFW_REPEAT) {
        EventManager::Get().Dispatch(KeyPressedEvent(key, true));
    }
}

static void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        EventManager::Get().Dispatch(MouseButtonPressedEvent(button));
    } else if (action == GLFW_RELEASE) {
        EventManager::Get().Dispatch(MouseButtonReleasedEvent(button));
    }
}

static void GLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    EventManager::Get().Dispatch(MouseMovedEvent(static_cast<float>(xpos), static_cast<float>(ypos)));
}

static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    EventManager::Get().Dispatch(MouseScrolledEvent(static_cast<float>(xoffset), static_cast<float>(yoffset)));
}

// ============================================================================
// Window Implementation
// ============================================================================

Window::Window(const WindowConfig& config)
    : m_Config(config)
    , m_Window(nullptr)
    , m_Initialized(false)
{
}

Window::~Window() {
    if (m_Initialized) {
        Shutdown();
    }
}

bool Window::Initialize() {
    NILOS_INFO("Initializing window...");

    // Initialize GLFW
    if (!glfwInit()) {
        NILOS_CRITICAL("Failed to initialize GLFW");
        return false;
    }

    // Set error callback
    glfwSetErrorCallback(GLFWErrorCallback);

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, m_Config.Resizable ? GLFW_TRUE : GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    // Create window
    GLFWmonitor* monitor = m_Config.Fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_Window = glfwCreateWindow(
        m_Config.Width,
        m_Config.Height,
        m_Config.Title.c_str(),
        monitor,
        nullptr
    );

    if (!m_Window) {
        NILOS_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    // Make OpenGL context current
    glfwMakeContextCurrent(m_Window);

    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NILOS_CRITICAL("Failed to initialize GLAD");
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        return false;
    }

    // Set VSync
    SetVSync(m_Config.VSync);

    // Setup callbacks
    SetupCallbacks();

    // Configure OpenGL
    glViewport(0, 0, m_Config.Width, m_Config.Height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Log OpenGL info
    NILOS_INFO("OpenGL Info:");
    NILOS_INFO("  Vendor: ", (const char*)glGetString(GL_VENDOR));
    NILOS_INFO("  Renderer: ", (const char*)glGetString(GL_RENDERER));
    NILOS_INFO("  Version: ", (const char*)glGetString(GL_VERSION));

    m_Initialized = true;
    NILOS_INFO("Window initialized successfully");

    return true;
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
    m_Initialized = false;
    NILOS_INFO("Window shutdown");
}

bool Window::ShouldClose() const {
    return m_Window && glfwWindowShouldClose(m_Window);
}

void Window::SetShouldClose(bool shouldClose) {
    if (m_Window) {
        glfwSetWindowShouldClose(m_Window, shouldClose ? GLFW_TRUE : GLFW_FALSE);
    }
}

void Window::SetVSync(bool enabled) {
    m_Config.VSync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetupCallbacks() {
    glfwSetWindowCloseCallback(m_Window, GLFWWindowCloseCallback);
    glfwSetWindowSizeCallback(m_Window, GLFWWindowSizeCallback);
    glfwSetKeyCallback(m_Window, GLFWKeyCallback);
    glfwSetMouseButtonCallback(m_Window, GLFWMouseButtonCallback);
    glfwSetCursorPosCallback(m_Window, GLFWCursorPosCallback);
    glfwSetScrollCallback(m_Window, GLFWScrollCallback);
}

} // namespace Nilos

