# API Reference - Quick Guide

This is a quick reference for the most commonly used Nilos Engine APIs.

## Core Systems

### Logger

```cpp
#include "Core/Logger.h"

// Log messages
NILOS_TRACE("Verbose debug info");
NILOS_DEBUG("Debug information");
NILOS_INFO("General info");
NILOS_WARNING("Warning message");
NILOS_ERROR("Error message");
NILOS_CRITICAL("Critical error");

// Set minimum log level
Logger::Get().SetLogLevel(LogLevel::Info);
```

### Time

```cpp
#include "Core/Time.h"

// Get delta time
float dt = Time::Get().GetDeltaTime();

// Get total time
float totalTime = Time::Get().GetTotalTime();

// Get FPS
float fps = Time::Get().GetFPS();

// Time scaling (slow motion / fast forward)
Time::Get().SetTimeScale(0.5f);  // Half speed
float scaledDt = Time::Get().GetScaledDeltaTime();
```

## ECS (Entity Component System)

### World

```cpp
#include "ECS/World.h"

// Create world
auto world = std::make_unique<World>();
world->Initialize();

// Update (in game loop)
world->Update(deltaTime);

// Shutdown
world->Shutdown();
```

### Entities

```cpp
// Create entity
Entity player = world->CreateEntity("Player");

// Destroy entity
world->DestroyEntity(player);

// Get entity name
std::string name = world->GetEntityName(player);
```

### Components

```cpp
// Add component
auto* transform = world->AddComponent<TransformComponent>(entity);
transform->Position = glm::vec3(0, 0, 0);

// Get component
auto* transform = world->GetComponent<TransformComponent>(entity);

// Check if has component
bool hasTransform = world->HasComponent<TransformComponent>(entity);

// Remove component
world->RemoveComponent<TransformComponent>(entity);

// Query entities with component
auto entities = world->GetEntitiesWithComponent<MeshComponent>();
```

### Systems

```cpp
// Register system
auto* physicsSystem = world->RegisterSystem<PhysicsSystem>();

// Get system
auto* aiSystem = world->GetSystem<AISystem>();

// Enable/disable system
physicsSystem->SetEnabled(false);
bool isEnabled = physicsSystem->IsEnabled();
```

## Components

### TransformComponent

```cpp
struct TransformComponent {
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);  // Euler angles (degrees)
    glm::vec3 Scale = glm::vec3(1.0f);
    
    glm::mat4 GetModelMatrix() const;
};
```

### MeshComponent

```cpp
struct MeshComponent {
    std::vector<float> Vertices;
    std::vector<uint32_t> Indices;
    uint32_t VAO, VBO, EBO;
    glm::vec3 Color;
    
    void CreateCube();
    bool IsInitialized() const;
};
```

### CameraComponent

```cpp
struct CameraComponent {
    float FOV = 60.0f;
    float Near = 0.1f;
    float Far = 100.0f;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    
    glm::vec3 Front, Up, Right;
    glm::mat4 ViewMatrix, ProjectionMatrix;
    
    void UpdateVectors();
    void UpdateProjectionMatrix(float aspectRatio);
    glm::mat4 GetViewMatrix(const glm::vec3& position) const;
};
```

### RigidbodyComponent

```cpp
struct RigidbodyComponent {
    glm::vec3 Velocity = glm::vec3(0.0f);
    glm::vec3 AngularVelocity = glm::vec3(0.0f);
    float Mass = 1.0f;
    float Drag = 0.1f;
    float AngularDrag = 0.05f;
    bool UseGravity = true;
    bool IsKinematic = false;
};
```

## Events

### EventManager

```cpp
#include "Events/EventManager.h"

// Subscribe to event
uint32_t subId = EventManager::Get().Subscribe<CollisionEvent>(
    [](const CollisionEvent& event) {
        // Handle event
    }
);

// Dispatch event
EventManager::Get().Dispatch(WindowCloseEvent());

// Unsubscribe
EventManager::Get().Unsubscribe(subId);
```

### Built-in Events

```cpp
// Window events
WindowCloseEvent()
WindowResizeEvent(width, height)

// Input events
KeyPressedEvent(keyCode, isRepeat)
KeyReleasedEvent(keyCode)
MouseButtonPressedEvent(button)
MouseButtonReleasedEvent(button)
MouseMovedEvent(x, y)
MouseScrolledEvent(xOffset, yOffset)

// Physics events (placeholder)
CollisionEvent(entityA, entityB, contactPoint)
TriggerEnterEvent(trigger, other)

// AI events (placeholder)
AIPerceptionEvent(observer, target, distance)
```

## Input

### Input System

```cpp
#include "Input/Input.h"

// Keyboard
bool isWPressed = Input::Get().IsKeyDown(GLFW_KEY_W);
bool wasSpacePressed = Input::Get().IsKeyPressed(GLFW_KEY_SPACE);
bool wasSpaceReleased = Input::Get().IsKeyReleased(GLFW_KEY_SPACE);

// Mouse buttons
bool isLeftMouseDown = Input::Get().IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT);
bool wasRightClicked = Input::Get().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);

// Mouse position
glm::vec2 mousePos = Input::Get().GetMousePosition();
glm::vec2 mouseDelta = Input::Get().GetMouseDelta();
glm::vec2 scrollDelta = Input::Get().GetMouseScroll();
```

### GLFW Key Codes

```cpp
// Letters: GLFW_KEY_A through GLFW_KEY_Z
// Numbers: GLFW_KEY_0 through GLFW_KEY_9
// Function keys: GLFW_KEY_F1 through GLFW_KEY_F12

// Common keys
GLFW_KEY_SPACE
GLFW_KEY_ESCAPE
GLFW_KEY_ENTER
GLFW_KEY_TAB
GLFW_KEY_BACKSPACE
GLFW_KEY_LEFT_SHIFT
GLFW_KEY_LEFT_CONTROL
GLFW_KEY_LEFT_ALT

// Arrow keys
GLFW_KEY_UP
GLFW_KEY_DOWN
GLFW_KEY_LEFT
GLFW_KEY_RIGHT

// Mouse buttons
GLFW_MOUSE_BUTTON_LEFT
GLFW_MOUSE_BUTTON_RIGHT
GLFW_MOUSE_BUTTON_MIDDLE
```

## Rendering

### Renderer

```cpp
#include "Rendering/Renderer.h"

// Initialize
auto renderer = std::make_unique<Renderer>();
renderer->Initialize();

// Frame rendering
renderer->BeginFrame();
renderer->RenderMesh(mesh, transform, camera, cameraTransform);
renderer->EndFrame();

// Configuration
renderer->SetClearColor(0.1f, 0.15f, 0.2f, 1.0f);

// Shutdown
renderer->Shutdown();
```

### Shader

```cpp
#include "Rendering/Shader.h"

// Load from files
Shader shader;
shader.LoadFromFiles("assets/shaders/basic.vert", "assets/shaders/basic.frag");

// Load from source strings
shader.LoadFromSource(vertexSource, fragmentSource);

// Use shader
shader.Use();

// Set uniforms
shader.SetInt("uTexture", 0);
shader.SetFloat("uTime", time);
shader.SetVec3("uColor", glm::vec3(1, 0, 0));
shader.SetMat4("uModel", modelMatrix);

// Cleanup
shader.Delete();
```

### Mesh Factory

```cpp
#include "Rendering/Mesh.h"

// Create common shapes
MeshComponent cube = MeshFactory::CreateCube();
MeshComponent plane = MeshFactory::CreatePlane(10.0f, 10.0f);
MeshComponent sphere = MeshFactory::CreateSphere(1.0f, 32);
```

## Window

### Window

```cpp
#include "Window/Window.h"

// Create window
WindowConfig config;
config.Title = "My Game";
config.Width = 1920;
config.Height = 1080;
config.VSync = true;
config.Fullscreen = false;

auto window = std::make_unique<Window>(config);
window->Initialize();

// Main loop
while (!window->ShouldClose()) {
    window->PollEvents();
    
    // ... game logic ...
    
    window->SwapBuffers();
}

// Cleanup
window->Shutdown();
```

## Engine

### Engine Core

```cpp
#include "Core/Engine.h"

// Configure engine
EngineConfig config;
config.WindowTitle = "My Game";
config.WindowWidth = 1280;
config.WindowHeight = 720;
config.VSync = true;
config.ShowFPS = true;

// Create and run
Engine engine(config);
if (engine.Initialize()) {
    engine.Run();
}
```

## Math (GLM)

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Vectors
glm::vec3 position(0, 1, 0);
glm::vec3 direction = glm::normalize(target - position);
float distance = glm::length(target - position);
float dot = glm::dot(forward, direction);

// Matrices
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
model = glm::scale(model, glm::vec3(2, 2, 2));

// View matrix
glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, upVector);

// Projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
```

## Common Patterns

### Creating a Game Object

```cpp
Entity CreateGameObject(World* world, const glm::vec3& pos) {
    Entity obj = world->CreateEntity("GameObject");
    
    auto* transform = world->AddComponent<TransformComponent>(obj);
    transform->Position = pos;
    
    auto* mesh = world->AddComponent<MeshComponent>(obj);
    mesh->CreateCube();
    
    auto* rigidbody = world->AddComponent<RigidbodyComponent>(obj);
    rigidbody->Mass = 1.0f;
    
    return obj;
}
```

### Camera Movement

```cpp
// In update loop
float speed = 5.0f * deltaTime;

if (Input::Get().IsKeyDown(GLFW_KEY_W)) {
    cameraPos += cameraFront * speed;
}
if (Input::Get().IsKeyDown(GLFW_KEY_S)) {
    cameraPos -= cameraFront * speed;
}
if (Input::Get().IsKeyDown(GLFW_KEY_A)) {
    cameraPos -= cameraRight * speed;
}
if (Input::Get().IsKeyDown(GLFW_KEY_D)) {
    cameraPos += cameraRight * speed;
}
```

### Responding to Events

```cpp
EventManager::Get().Subscribe<CollisionEvent>([](const CollisionEvent& e) {
    NILOS_INFO("Collision between ", e.EntityA, " and ", e.EntityB);
});
```

---

For complete documentation, see:
- `docs/ECS_GUIDE.md` - Detailed ECS usage
- `ARCHITECTURE.md` - System architecture
- Source code comments - Inline documentation

**Last Updated**: December 2025

