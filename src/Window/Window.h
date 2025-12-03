#pragma once

#include <string>
#include <cstdint>

// Forward declare GLFW structures to avoid including GLFW in header
struct GLFWwindow;

namespace Nilos {

/**
 * @brief Window configuration structure
 */
struct WindowConfig {
    std::string Title = "Nilos Engine";
    uint32_t Width = 1280;
    uint32_t Height = 720;
    bool VSync = true;
    bool Fullscreen = false;
    bool Resizable = true;
};

/**
 * @brief Window management class using GLFW
 * 
 * Handles window creation, events, and OpenGL context.
 * This abstracts the windowing library so it can be swapped if needed.
 */
class Window {
public:
    explicit Window(const WindowConfig& config);
    ~Window();

    /**
     * @brief Initialize the window and OpenGL context
     */
    bool Initialize();

    /**
     * @brief Poll for events (keyboard, mouse, etc.)
     */
    void PollEvents();

    /**
     * @brief Swap front and back buffers
     */
    void SwapBuffers();

    /**
     * @brief Shutdown and destroy the window
     */
    void Shutdown();

    /**
     * @brief Check if window should close
     */
    bool ShouldClose() const;

    /**
     * @brief Set window should close flag
     */
    void SetShouldClose(bool shouldClose);

    // Getters
    uint32_t GetWidth() const { return m_Config.Width; }
    uint32_t GetHeight() const { return m_Config.Height; }
    float GetAspectRatio() const { 
        return static_cast<float>(m_Config.Width) / static_cast<float>(m_Config.Height); 
    }
    GLFWwindow* GetNativeWindow() const { return m_Window; }

    /**
     * @brief Set VSync on/off
     */
    void SetVSync(bool enabled);

    /**
     * @brief Get VSync state
     */
    bool IsVSyncEnabled() const { return m_Config.VSync; }

private:
    /**
     * @brief Setup GLFW callbacks for events
     */
    void SetupCallbacks();

    WindowConfig m_Config;
    GLFWwindow* m_Window;
    bool m_Initialized;
};

} // namespace Nilos

