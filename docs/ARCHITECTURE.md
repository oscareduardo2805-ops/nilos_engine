# Nilos Engine Architecture

This document describes the architectural decisions and design patterns used in Nilos Engine.

## Core Design Philosophy

1. **Modularity**: Each subsystem is independent and loosely coupled
2. **Extensibility**: New features can be added without modifying core code
3. **Performance**: Architecture supports future optimizations (ECS, data-oriented design)
4. **Clarity**: Code is readable and well-documented for learning and collaboration

## Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                    Application Layer                     │
│                      (main.cpp)                          │
└───────────────────────┬─────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────┐
│                     Engine Core                          │
│          (Initialization, Main Loop, Timing)             │
└─┬─────────┬──────────┬──────────┬──────────┬───────────┘
  │         │          │          │          │
  │         │          │          │          │
┌─▼─────┐ ┌▼──────┐ ┌─▼──────┐ ┌─▼──────┐ ┌▼───────────┐
│Window │ │ Input │ │Renderer│ │  ECS   │ │   Events   │
│ GLFW  │ │System │ │OpenGL  │ │ World  │ │  Manager   │
└───────┘ └───────┘ └────────┘ └─┬──────┘ └────────────┘
                                  │
                    ┌─────────────┼─────────────┐
                    │             │             │
              ┌─────▼──────┐ ┌───▼────┐ ┌─────▼─────┐
              │  Physics   │ │   AI   │ │ Rendering │
              │   System   │ │ System │ │  System   │
              │(Placeholder)│ │(Place.)│ │ (Future)  │
              └────────────┘ └────────┘ └───────────┘
```

## Entity Component System (ECS)

### Why ECS?

Traditional Object-Oriented Programming (OOP) with inheritance hierarchies becomes unwieldy for games:

```
GameObject
├── StaticObject
│   ├── Tree
│   └── Rock
└── DynamicObject
    ├── Character
    │   ├── Player
    │   └── NPC
    └── Projectile
```

Problems:
- Deep inheritance hierarchies are rigid
- Multiple inheritance is messy (diamond problem)
- Hard to add cross-cutting features (e.g., "all objects that can burn")
- Poor cache coherency (OOP scatters data)

### ECS Solution

**Entities**: Just IDs (lightweight)
```cpp
Entity player = world->CreateEntity("Player");
```

**Components**: Pure data (no logic)
```cpp
struct TransformComponent {
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};
```

**Systems**: Pure logic (no data)
```cpp
class PhysicsSystem : public System {
    void Update(float dt) {
        // Operate on all entities with Transform + Rigidbody
    }
};
```

### Benefits

1. **Flexibility**: Mix and match components freely
2. **Performance**: Cache-friendly data layout
3. **Maintainability**: Clear separation of concerns
4. **Scalability**: Easy to parallelize systems

### Example Usage

```cpp
// Create an entity
Entity npc = world->CreateEntity("Guard");

// Add components
auto* transform = world->AddComponent<TransformComponent>(npc);
transform->Position = glm::vec3(0, 0, 0);

auto* ai = world->AddComponent<AIAgentComponent>(npc);
ai->PerceptionRadius = 10.0f;

auto* rigidbody = world->AddComponent<RigidbodyComponent>(npc);
rigidbody->Mass = 80.0f;

// Systems automatically process entities with required components
```

## Event System

### Purpose

Decouple modules that need to communicate. Instead of direct dependencies:

```cpp
// BAD: Tight coupling
if (collision) {
    audioSystem.PlaySound("impact");
    particleSystem.Emit("sparks");
}
```

Use events:

```cpp
// GOOD: Loose coupling
EventManager::Get().Dispatch(CollisionEvent(entityA, entityB));
```

Subscribers react independently:

```cpp
// In AudioSystem
EventManager::Get().Subscribe<CollisionEvent>([](const CollisionEvent& e) {
    PlaySound("impact");
});

// In ParticleSystem
EventManager::Get().Subscribe<CollisionEvent>([](const CollisionEvent& e) {
    EmitSparks(e.ContactPoint);
});
```

### Benefits

- Modules don't need to know about each other
- Easy to add/remove features
- Event history can be logged/replayed
- Supports undo/redo systems

## Module Breakdown

### Core Module
- **Engine**: Main loop, initialization, shutdown
- **Logger**: Console logging with levels
- **Time**: Delta time, FPS, time scaling

### Window Module
- **Window**: GLFW wrapper, OpenGL context
- Abstracts windowing library (could swap GLFW for SDL)

### Input Module
- **Input**: Keyboard and mouse state queries
- Frame-by-frame state tracking (pressed vs held)

### Rendering Module
- **Renderer**: Draw call management
- **Shader**: GLSL compilation and uniform setting
- **Camera**: Projection and view matrices
- **Mesh**: Vertex data and GPU buffers

### ECS Module
- **World**: Entity and component storage
- **Entity**: Unique identifiers
- **Component**: Data structures (Transform, Mesh, etc.)
- **System**: Logic processors (Physics, AI, etc.)

### Events Module
- **EventManager**: Subscribe/dispatch events
- **Event**: Base class for all events

### Physics Module (Placeholder)
- Future: Rigid bodies, collision detection, constraints
- Integration point for PhysX/Bullet

### AI Module (Placeholder)
- Future: Behavior trees, GOAP, perception, memory
- Integration point for external AI APIs

## Data Flow

### Frame Update Loop

```
1. Poll window events (GLFW)
2. Update input state
3. Update time (delta time calculation)
4. Process queued events
5. Update all systems (Physics, AI, Rendering, etc.)
6. Render frame
   a. Begin frame (clear buffers)
   b. For each mesh entity:
      - Get transform and mesh components
      - Set shader uniforms
      - Draw mesh
   c. End frame
7. Swap buffers
8. Repeat
```

### Rendering Pipeline

```
CPU Side:
  Entity with Transform + Mesh → Renderer
  ↓
  Set MVP matrices as uniforms
  ↓
  Issue draw call

GPU Side:
  Vertex Shader (transform vertices)
  ↓
  Rasterization
  ↓
  Fragment Shader (color pixels)
  ↓
  Framebuffer
```

## Future Architecture Expansions

### Phase 2: Enhanced Rendering
- **Render Queue**: Sort by material/distance
- **Material System**: PBR properties
- **Texture Manager**: Loading and caching
- **Lighting System**: Point/directional/spot lights

### Phase 3: Physics Integration
- **Collision Shapes**: Box, sphere, capsule, mesh
- **Broad Phase**: Spatial partitioning (octree/BVH)
- **Narrow Phase**: SAT, GJK, EPA algorithms
- **Solver**: Contact constraints, joints

### Phase 4: Networking (if needed)
- **Client-Server**: Authority-based replication
- **Entity Replication**: Selective component sync
- **Rollback**: For lag compensation
- **Interest Management**: Relevancy filtering

### Phase 5: AI Systems
- **Behavior Trees**: Visual editor, nodes
- **GOAP Planner**: A* search over actions
- **Perception**: Raycast vision, sound propagation
- **Memory**: Vector database (FAISS/Milvus)
- **API Client**: REST calls to LLM providers

### Phase 6: Procedural World
- **Voxel Engine**: Chunk-based world
- **Procedural Generation**: Noise functions, rules
- **World Modification**: Dynamic building/destruction
- **Persistence**: Save/load world state

## Performance Considerations

### Current Implementation
- Simple vector storage for components (O(n) queries)
- Immediate mode rendering (one draw call per mesh)
- Single-threaded

### Future Optimizations
- **Component Storage**: Sparse sets or archetypes
- **Instanced Rendering**: Batch similar meshes
- **Multi-threading**: Job system for parallel system updates
- **Spatial Partitioning**: Octree/BVH for culling
- **LOD System**: Level of detail for distant objects

## Design Patterns Used

1. **Singleton**: Logger, EventManager, Time, Input
   - Justified for global utilities
   
2. **Factory**: MeshFactory for creating common shapes
   
3. **Observer**: Event system (pub/sub)
   
4. **Component**: ECS components are pure data
   
5. **Strategy**: Systems define different behaviors
   
6. **Facade**: Engine class hides subsystem complexity

## Code Style Guidelines

1. **PascalCase** for classes, structs, enums
2. **PascalCase** for public methods
3. **camelCase** for local variables and parameters
4. **m_** prefix for member variables
5. **s_** prefix for static variables
6. **I** prefix for interfaces (if used)
7. **Comprehensive comments** for all public APIs
8. **Doxygen-style** documentation

## Error Handling

Currently using:
- Boolean return values for initialization
- Logging for errors
- Assertions for debug builds (future)

Future:
- Exception handling where appropriate
- Error codes for performance-critical paths
- Graceful degradation

## Dependencies

### Required
- **C++17**: Modern C++ features
- **CMake**: Cross-platform build system
- **OpenGL 3.3+**: Graphics API
- **GLFW3**: Windowing and input
- **GLM**: Math library
- **GLAD**: OpenGL loader

### Future
- **PhysX or Bullet**: Physics engine
- **OpenAL**: Audio system
- **Assimp**: Model loading
- **stb_image**: Texture loading
- **ImGui**: Debug UI
- **spdlog**: Better logging (optional)

## Testing Strategy (Future)

1. **Unit Tests**: Core utilities, math functions
2. **Integration Tests**: System interactions
3. **Performance Tests**: Frame time, memory usage
4. **Visual Tests**: Screenshot comparison

---

**Last Updated**: December 2025  
**Version**: 0.1.0 (Phase 1)

