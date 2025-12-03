#pragma once

/**
 * @file NilosEngine.h
 * @brief Main public API header for Nilos Engine
 * 
 * Include this file in your project to access engine functionality.
 * This is the only header external projects need to include.
 */

// Core systems
#include "../src/Core/Engine.h"
#include "../src/Core/Logger.h"
#include "../src/Core/Time.h"

// ECS
#include "../src/ECS/Entity.h"
#include "../src/ECS/Component.h"
#include "../src/ECS/System.h"
#include "../src/ECS/World.h"

// Events
#include "../src/Events/Event.h"
#include "../src/Events/EventManager.h"

// Window and Input
#include "../src/Window/Window.h"
#include "../src/Input/Input.h"

// Rendering
#include "../src/Rendering/Renderer.h"
#include "../src/Rendering/Shader.h"
#include "../src/Rendering/Camera.h"
#include "../src/Rendering/Mesh.h"
#include "../src/Rendering/Material.h"

// Systems
#include "../src/Physics/PhysicsSystem.h"
#include "../src/AI/AISystem.h"

/**
 * @namespace Nilos
 * @brief Main namespace for the Nilos Engine
 */
namespace Nilos {

/**
 * @brief Engine version information
 */
struct Version {
    static constexpr int MAJOR = 0;
    static constexpr int MINOR = 1;
    static constexpr int PATCH = 0;
    static constexpr const char* STRING = "0.1.0";
    static constexpr const char* NAME = "Nilos Engine - Phase 1";
};

} // namespace Nilos

