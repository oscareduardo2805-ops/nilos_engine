#pragma once

#include <memory>
#include <string>

namespace Nilos {

// Forward declarations
class Window;
class Renderer;
class World;

/**
 * @brief Engine configuration structure
 */
struct EngineConfig {
    std::string WindowTitle = "Nilos Engine";
    uint32_t WindowWidth = 1280;
    uint32_t WindowHeight = 720;
    bool VSync = true;
    bool Fullscreen = false;
    uint32_t TargetFPS = 60;
    bool ShowFPS = true;
};

/**
 * @brief Main engine class that coordinates all subsystems
 * 
 * The Engine class is responsible for:
 * - Initializing all engine subsystems (rendering, physics, audio, etc.)
 * - Running the main game loop
 * - Managing the frame rate
 * - Coordinating communication between subsystems
 * 
 * This is the heart of the engine's architecture. New systems should be
 * registered here and updated in the main loop.
 */
class Engine {
public:
    /**
     * @brief Construct the engine with a configuration
     */
    explicit Engine(const EngineConfig& config = EngineConfig());
    
    /**
     * @brief Destructor - cleanup all subsystems
     */
    ~Engine();

    /**
     * @brief Initialize all engine subsystems
     * @return True if initialization successful, false otherwise
     */
    bool Initialize();

    /**
     * @brief Run the main game loop
     * 
     * This will block until the application is closed.
     * The loop handles: input, update, render, and frame timing.
     */
    void Run();

    /**
     * @brief Shutdown all engine subsystems
     */
    void Shutdown();

    /**
     * @brief Request the engine to stop running
     */
    void RequestShutdown() { m_Running = false; }

    /**
     * @brief Check if the engine is currently running
     */
    bool IsRunning() const { return m_Running; }

    // Subsystem accessors
    Window* GetWindow() const { return m_Window.get(); }
    Renderer* GetRenderer() const { return m_Renderer.get(); }
    World* GetWorld() const { return m_World.get(); }

private:
    /**
     * @brief Process input events
     */
    void ProcessInput();

    /**
     * @brief Update all systems with delta time
     */
    void Update(float deltaTime);

    /**
     * @brief Render the current frame
     */
    void Render();

    /**
     * @brief Setup the demo scene (for Phase 1)
     */
    void SetupDemoScene();

    EngineConfig m_Config;
    bool m_Running;
    bool m_Initialized;

    // Core subsystems
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<World> m_World;

    // Demo scene entities (Phase 1)
    uint32_t m_CameraEntity;
    uint32_t m_CubeEntity;
};

} // namespace Nilos

