#include "Engine.h"
#include "Logger.h"
#include "Time.h"
#include "../Window/Window.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Camera.h"
#include "../ECS/World.h"
#include "../ECS/Component.h"
#include "../Input/Input.h"
#include "../Events/EventManager.h"
#include "../Events/Event.h"
#include "../Physics/PhysicsWorld.h"

#include <GLFW/glfw3.h>
#include <thread>

namespace Nilos {

Engine::Engine(const EngineConfig& config)
    : m_Config(config)
    , m_Running(false)
    , m_Initialized(false)
    , m_CameraEntity(0)
    , m_CubeEntity(0)
{
}

Engine::~Engine() {
    if (m_Initialized) {
        Shutdown();
    }
}

bool Engine::Initialize() {
    NILOS_INFO("=== Nilos Engine Initialization ===");
    
    // Initialize time system
    Time::Get().Initialize();
    NILOS_INFO("Time system initialized");

    // Initialize event system
    EventManager::Get().Initialize();
    NILOS_INFO("Event system initialized");

    // Create window
    WindowConfig windowConfig;
    windowConfig.Title = m_Config.WindowTitle;
    windowConfig.Width = m_Config.WindowWidth;
    windowConfig.Height = m_Config.WindowHeight;
    windowConfig.VSync = m_Config.VSync;
    windowConfig.Fullscreen = m_Config.Fullscreen;

    m_Window = std::make_unique<Window>(windowConfig);
    if (!m_Window->Initialize()) {
        NILOS_CRITICAL("Failed to initialize window");
        return false;
    }
    NILOS_INFO("Window created: ", m_Config.WindowWidth, "x", m_Config.WindowHeight);

    // Initialize input system
    Input::Get().Initialize(m_Window->GetNativeWindow());
    NILOS_INFO("Input system initialized");

    // Create renderer
    m_Renderer = std::make_unique<Renderer>();
    if (!m_Renderer->Initialize()) {
        NILOS_CRITICAL("Failed to initialize renderer");
        return false;
    }
    NILOS_INFO("Renderer initialized");

    // Create ECS world
    m_World = std::make_unique<World>();
    m_World->Initialize();
    NILOS_INFO("ECS World initialized");

    // Initialize Physics World (Phase 3)
    m_PhysicsWorld = std::make_unique<PhysicsWorld>();
    m_PhysicsWorld->SetGravity(glm::vec3(0.0f, -9.81f, 0.0f));
    NILOS_INFO("Physics World initialized");

    // Setup demo scene
    SetupDemoScene();
    NILOS_INFO("Demo scene created");

    // Subscribe to window events
    EventManager::Get().Subscribe<WindowCloseEvent>([this](const WindowCloseEvent&) {
        RequestShutdown();
    });

    m_Initialized = true;
    NILOS_INFO("=== Engine Initialization Complete ===");
    
    return true;
}

void Engine::Run() {
    if (!m_Initialized) {
        NILOS_ERROR("Cannot run engine: not initialized");
        return;
    }

    NILOS_INFO("Starting main loop...");
    m_Running = true;

    float frameTimeAccumulator = 0.0f;
    uint32_t frameCountForFPS = 0;

    while (m_Running && !m_Window->ShouldClose()) {
        // Update time
        Time::Get().Update();
        float deltaTime = Time::Get().GetDeltaTime();

        // Display FPS periodically
        if (m_Config.ShowFPS) {
            frameTimeAccumulator += deltaTime;
            frameCountForFPS++;
            if (frameTimeAccumulator >= 1.0f) {
                float fps = Time::Get().GetFPS();
                NILOS_DEBUG("FPS: ", static_cast<int>(fps), " | Frame time: ", 
                           (deltaTime * 1000.0f), "ms");
                frameTimeAccumulator = 0.0f;
            }
        }

        // Process input
        ProcessInput();

        // Update all systems
        Update(deltaTime);

        // Render
        Render();

        // Poll window events
        m_Window->PollEvents();
    }

    NILOS_INFO("Main loop ended");
}

void Engine::Shutdown() {
    NILOS_INFO("=== Engine Shutdown ===");

    if (m_World) {
        m_World->Shutdown();
        m_World.reset();
    }

    if (m_Renderer) {
        m_Renderer->Shutdown();
        m_Renderer.reset();
    }

    if (m_Window) {
        m_Window->Shutdown();
        m_Window.reset();
    }

    EventManager::Get().Shutdown();

    m_Initialized = false;
    NILOS_INFO("=== Engine Shutdown Complete ===");
}

void Engine::ProcessInput() {
    Input::Get().Update();

    // ESC to quit
    if (Input::Get().IsKeyPressed(GLFW_KEY_ESCAPE)) {
        RequestShutdown();
    }

    // Camera controls
    auto* transform = m_World->GetComponent<TransformComponent>(m_CameraEntity);
    auto* camera = m_World->GetComponent<CameraComponent>(m_CameraEntity);
    
    if (transform && camera) {
        float baseSpeed = 5.0f; // Base speed: 5 units per second
        float speedMultiplier = 1.0f;
        
        // SPRINT MODE: Ctrl + Movement = 3x speed
        if (Input::Get().IsKeyDown(GLFW_KEY_LEFT_CONTROL) || 
            Input::Get().IsKeyDown(GLFW_KEY_RIGHT_CONTROL)) {
            speedMultiplier = 3.0f;
        }
        
        float moveSpeed = baseSpeed * speedMultiplier * Time::Get().GetDeltaTime();
        float rotationSpeed = 2.0f * Time::Get().GetDeltaTime();

        // Movement (WASD + Q/E for vertical)
        if (Input::Get().IsKeyDown(GLFW_KEY_W)) {
            transform->Position += camera->GetForward() * moveSpeed;
        }
        if (Input::Get().IsKeyDown(GLFW_KEY_S)) {
            transform->Position -= camera->GetForward() * moveSpeed;
        }
        if (Input::Get().IsKeyDown(GLFW_KEY_A)) {
            transform->Position -= camera->GetRight() * moveSpeed;
        }
        if (Input::Get().IsKeyDown(GLFW_KEY_D)) {
            transform->Position += camera->GetRight() * moveSpeed;
        }
        if (Input::Get().IsKeyDown(GLFW_KEY_Q)) {
            transform->Position.y -= moveSpeed;
        }
        if (Input::Get().IsKeyDown(GLFW_KEY_E)) {
            transform->Position.y += moveSpeed;
        }

        // Mouse look (only when right mouse button held)
        if (Input::Get().IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
            glm::vec2 mouseDelta = Input::Get().GetMouseDelta();
            camera->Yaw += mouseDelta.x * rotationSpeed;
            camera->Pitch -= mouseDelta.y * rotationSpeed;
            
            // Clamp pitch to prevent gimbal lock
            if (camera->Pitch > 89.0f) camera->Pitch = 89.0f;
            if (camera->Pitch < -89.0f) camera->Pitch = -89.0f;
        }
    }
}

void Engine::Update(float deltaTime) {
    // Update ECS world (all registered systems)
    m_World->Update(deltaTime);

    // Update physics (Phase 3)
    m_PhysicsWorld->Update(deltaTime);

    // Update camera
    auto* camera = m_World->GetComponent<CameraComponent>(m_CameraEntity);
    if (camera) {
        camera->UpdateVectors();
    }

    // No automatic rotation - objects only move with physics
    // Physical objects are controlled by RigidbodyComponent
}

void Engine::Render() {
    // Get camera data
    auto* cameraTransform = m_World->GetComponent<TransformComponent>(m_CameraEntity);
    auto* camera = m_World->GetComponent<CameraComponent>(m_CameraEntity);

    if (cameraTransform && camera) {
        // Update camera matrices
        float aspect = static_cast<float>(m_Config.WindowWidth) / 
                      static_cast<float>(m_Config.WindowHeight);
        camera->UpdateProjectionMatrix(aspect);

        // Begin frame
        m_Renderer->BeginFrame();

        // Render all entities with MeshComponent
        auto entities = m_World->GetEntitiesWithComponent<MeshComponent>();
        for (uint32_t entity : entities) {
            auto* mesh = m_World->GetComponent<MeshComponent>(entity);
            auto* transform = m_World->GetComponent<TransformComponent>(entity);
            
            if (mesh && transform) {
                m_Renderer->RenderMesh(*mesh, *transform, *camera, *cameraTransform);
            }
        }

        // End frame
        m_Renderer->EndFrame();
    }

    // Swap buffers
    m_Window->SwapBuffers();
}

void Engine::SetupDemoScene() {
    // Setup Phong lighting (Phase 2 feature)
    DirectionalLight dirLight;
    dirLight.Direction = glm::normalize(glm::vec3(-1.0f, -1.2f, -0.8f));
    dirLight.Color = glm::vec3(1.0f, 0.95f, 0.85f); // Warm sunlight
    dirLight.Intensity = 1.8f; // Stronger light to see effect
    m_Renderer->SetDirectionalLight(dirLight);
    
    AmbientLight ambLight;
    ambLight.Color = glm::vec3(0.15f, 0.18f, 0.25f); // Cool ambient
    ambLight.Intensity = 0.2f; // Low ambient for dramatic lighting
    m_Renderer->SetAmbientLight(ambLight);
    
    NILOS_INFO("Phong lighting configured: Directional + Ambient");
    
    // Create camera entity
    m_CameraEntity = m_World->CreateEntity("MainCamera");
    
    auto* cameraTransform = m_World->AddComponent<TransformComponent>(m_CameraEntity);
    cameraTransform->Position = glm::vec3(0.0f, 50.0f, 150.0f); // Higher and farther to see 1km plane
    
    auto* cameraComponent = m_World->AddComponent<CameraComponent>(m_CameraEntity);
    cameraComponent->FOV = 70.0f;       // Wider FOV to see more
    cameraComponent->Near = 0.1f;
    cameraComponent->Far = 2000.0f;     // Far enough to see 1km plane

    // ========================================
    // GROUND (Static - Earth simulation with depth)
    // ========================================
    Entity ground = m_World->CreateEntity("Ground");
    auto* groundTransform = m_World->AddComponent<TransformComponent>(ground);
    groundTransform->Position = glm::vec3(0.0f, -2.0f, 0.0f); // 2m below surface
    groundTransform->Scale = glm::vec3(1000.0f, 4.0f, 1000.0f); // 1km x 4m deep x 1km
    
    auto* groundMesh = m_World->AddComponent<MeshComponent>(ground);
    groundMesh->CreateCube();
    // Override color to be uniform green (grass)
    groundMesh->Color = glm::vec3(0.2f, 0.6f, 0.2f);
    
    auto* groundCollider = m_World->AddComponent<ColliderComponent>(ground);
    groundCollider->ColliderType = ColliderComponent::Type::Box;
    groundCollider->Size = glm::vec3(1.0f); // Will be scaled by transform
    
    // Ground is static (never moves)
    m_PhysicsWorld->RegisterStaticCollider(groundCollider, groundTransform);
    
    // ========================================
    // BASKETBALL (Dynamic - Falls with realistic physics)
    // ========================================
    Entity basketball = m_World->CreateEntity("Basketball");
    auto* ballTransform = m_World->AddComponent<TransformComponent>(basketball);
    ballTransform->Position = glm::vec3(0.0f, 5.0f, 0.0f); // 5 meters high
    ballTransform->Scale = glm::vec3(0.24f); // Basketball diameter 24cm
    
    auto* ballMesh = m_World->AddComponent<MeshComponent>(basketball);
    ballMesh->CreateSphere(0.5f, 32, 16);
    
    auto* ballRB = m_World->AddComponent<RigidbodyComponent>(basketball);
    ballRB->SetMass(0.62f); // Basketball mass: 620 grams
    ballRB->UseGravity = true;
    ballRB->Restitution = 0.75f; // Basketball bounce
    ballRB->DynamicFriction = 0.3f;
    ballRB->StaticFriction = 0.5f;
    
    auto* ballCollider = m_World->AddComponent<ColliderComponent>(basketball);
    ballCollider->ColliderType = ColliderComponent::Type::Sphere;
    ballCollider->Radius = 0.5f; // Sphere mesh is 0.5 radius, scaled by transform (0.24)
    // Note: Size is ignored for spheres, only Radius is used
    
    m_PhysicsWorld->RegisterRigidbody(ballRB, ballCollider, ballTransform);
    
    // ========================================
    // LEFT CUBE (Dynamic - Falls from 3m)
    // ========================================
    Entity leftCube = m_World->CreateEntity("LeftCube");
    auto* leftTransform = m_World->AddComponent<TransformComponent>(leftCube);
    leftTransform->Position = glm::vec3(-3.0f, 3.0f, 0.0f); // 3m high
    leftTransform->Scale = glm::vec3(1.0f);
    
    m_World->AddComponent<MeshComponent>(leftCube)->CreateCube();
    
    auto* leftRB = m_World->AddComponent<RigidbodyComponent>(leftCube);
    leftRB->SetMass(10.0f); // 10 kg wooden crate
    leftRB->UseGravity = true;
    leftRB->Restitution = 0.3f; // Less bouncy than basketball
    leftRB->DynamicFriction = 0.5f;
    
    auto* leftCollider = m_World->AddComponent<ColliderComponent>(leftCube);
    leftCollider->ColliderType = ColliderComponent::Type::Box;  // CRITICAL: Set collider type!
    leftCollider->Size = glm::vec3(1.0f);
    
    m_PhysicsWorld->RegisterRigidbody(leftRB, leftCollider, leftTransform);
    
    // ========================================
    // RIGHT CUBE (Dynamic - Falls from 4m)
    // ========================================
    Entity rightCube = m_World->CreateEntity("RightCube");
    auto* rightTransform = m_World->AddComponent<TransformComponent>(rightCube);
    rightTransform->Position = glm::vec3(3.0f, 4.0f, 0.0f); // 4m high
    rightTransform->Scale = glm::vec3(1.0f);
    
    m_World->AddComponent<MeshComponent>(rightCube)->CreateCube();
    
    auto* rightRB = m_World->AddComponent<RigidbodyComponent>(rightCube);
    rightRB->SetMass(10.0f);
    rightRB->UseGravity = true;
    rightRB->Restitution = 0.3f;
    rightRB->DynamicFriction = 0.5f;
    
    auto* rightCollider = m_World->AddComponent<ColliderComponent>(rightCube);
    rightCollider->ColliderType = ColliderComponent::Type::Box;  // CRITICAL: Set collider type!
    rightCollider->Size = glm::vec3(1.0f);
    
    m_PhysicsWorld->RegisterRigidbody(rightRB, rightCollider, rightTransform);
    
    // Center cube is now the demo cube entity
    m_CubeEntity = leftCube;
    
    NILOS_INFO("Realistic physics scene created:");
    NILOS_INFO("  - Ground: 1km² x 4m deep (static)");
    NILOS_INFO("  - Basketball: 0.62kg at 5m (bounces 0.75)");
    NILOS_INFO("  - Cubes: 10kg at 3-4m (bounces 0.3)");
}

} // namespace Nilos

