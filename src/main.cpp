/**
 * @file main.cpp
 * @brief Entry point for Nilos Engine
 * 
 * This demonstrates basic engine usage. In a real game, you would:
 * 1. Create a custom Application class inheriting from Engine
 * 2. Override setup/update methods with your game logic
 * 3. Use the ECS to create and manage game entities
 * 
 * Example game structure:
 *   class MyGame : public Application {
 *       void OnStartup() override { ... }
 *       void OnUpdate(float dt) override { ... }
 *       void OnShutdown() override { ... }
 *   };
 */

#include "Core/Engine.h"
#include "Core/Logger.h"

int main(int argc, char** argv) {
    // Configure engine
    Nilos::EngineConfig config;
    config.WindowTitle = "Nilos Engine - Phase 1 Demo";
    config.WindowWidth = 1280;
    config.WindowHeight = 720;
    config.VSync = true;
    config.ShowFPS = true;

    // Create and initialize engine
    Nilos::Engine engine(config);
    
    if (!engine.Initialize()) {
        NILOS_CRITICAL("Failed to initialize engine!");
        return -1;
    }

    // Run the engine
    NILOS_INFO("=== Nilos Engine Running ===");
    NILOS_INFO("Controls:");
    NILOS_INFO("  W/A/S/D - Move camera forward/left/backward/right");
    NILOS_INFO("  Q/E - Move camera down/up");
    NILOS_INFO("  Right Mouse Button + Move - Look around");
    NILOS_INFO("  ESC - Exit");
    NILOS_INFO("");
    
    engine.Run();

    // Cleanup happens automatically in destructor
    NILOS_INFO("Engine exited cleanly");
    
    return 0;
}

