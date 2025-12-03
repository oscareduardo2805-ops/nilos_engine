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
        float moveSpeed = 5.0f * Time::Get().GetDeltaTime();
        float rotationSpeed = 2.0f * Time::Get().GetDeltaTime();

        // Movement
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

    // Update camera
    auto* camera = m_World->GetComponent<CameraComponent>(m_CameraEntity);
    if (camera) {
        camera->UpdateVectors();
    }

    // Rotate the demo cube
    auto* cubeTransform = m_World->GetComponent<TransformComponent>(m_CubeEntity);
    if (cubeTransform) {
        float rotationSpeed = 45.0f; // degrees per second
        cubeTransform->Rotation.y += rotationSpeed * deltaTime;
        cubeTransform->Rotation.x += rotationSpeed * 0.5f * deltaTime;
    }
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
    cameraTransform->Position = glm::vec3(0.0f, 2.5f, 7.0f);
    
    auto* cameraComponent = m_World->AddComponent<CameraComponent>(m_CameraEntity);
    cameraComponent->FOV = 60.0f;
    cameraComponent->Near = 0.1f;
    cameraComponent->Far = 100.0f;

    // Create center cube (rotating to show lighting)
    m_CubeEntity = m_World->CreateEntity("DemoCube");
    
    auto* cubeTransform = m_World->AddComponent<TransformComponent>(m_CubeEntity);
    cubeTransform->Position = glm::vec3(0.0f, 0.0f, 0.0f);
    cubeTransform->Scale = glm::vec3(1.5f);
    
    auto* cubeMesh = m_World->AddComponent<MeshComponent>(m_CubeEntity);
    cubeMesh->CreateCube();
    
    // Add more cubes to showcase lighting
    Entity leftCube = m_World->CreateEntity("LeftCube");
    auto* leftTransform = m_World->AddComponent<TransformComponent>(leftCube);
    leftTransform->Position = glm::vec3(-3.5f, 0.0f, -1.0f);
    leftTransform->Scale = glm::vec3(1.0f);
    m_World->AddComponent<MeshComponent>(leftCube)->CreateCube();
    
    Entity rightCube = m_World->CreateEntity("RightCube");
    auto* rightTransform = m_World->AddComponent<TransformComponent>(rightCube);
    rightTransform->Position = glm::vec3(3.5f, 0.0f, -1.0f);
    rightTransform->Scale = glm::vec3(1.0f);
    m_World->AddComponent<MeshComponent>(rightCube)->CreateCube();
    
    // Ground plane (to see shadows effect)
    Entity ground = m_World->CreateEntity("Ground");
    auto* groundTransform = m_World->AddComponent<TransformComponent>(ground);
    groundTransform->Position = glm::vec3(0.0f, -2.5f, 0.0f);
    groundTransform->Scale = glm::vec3(15.0f, 0.2f, 15.0f);
    m_World->AddComponent<MeshComponent>(ground)->CreateCube();
    
    NILOS_INFO("Phase 2 demo scene created with Phong lighting!");
}

} // namespace Nilos

