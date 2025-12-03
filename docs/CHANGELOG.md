# Changelog

All notable changes to Nilos Engine will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.0] - 2025-12-03

### Added - Phase 2: Enhanced Rendering

#### Texture System
- **stb_image Integration**: Industry-standard image loading library
- **Texture2D Class**: Complete texture management with filtering and wrapping
- **TextureManager**: Automatic texture caching to prevent duplicate loads
- **Mipmap Support**: Automatic mipmap generation for better quality at distance
- **Multiple Formats**: RGB, RGBA, grayscale support

#### Lighting System
- **Phong Lighting Model**: Fast and simple lighting (Blinn-Phong variant)
- **DirectionalLight**: Infinite distance sun-like lights with direction and color
- **PointLight**: Local lights with position and attenuation (falloff)
- **AmbientLight**: Global ambient illumination
- **Per-Fragment Lighting**: Smooth lighting calculated per pixel

#### Material System
- **Simplified Materials**: Diffuse + Specular + Shininess (Phong model)
- **Texture Mapping**: Support for diffuse, specular, and normal maps
- **Material Component**: Easy-to-use material struct

#### Vertex Format
- **Enhanced Vertex Data**: Position(3) + Normal(3) + Color(3) + TexCoord(2)
- **Normal Vectors**: Required for proper lighting calculations
- **Texture Coordinates**: Ready for texture mapping

### Changed
- **Shaders**: Upgraded from basic vertex color to full Phong lighting
- **Mesh Creation**: Cube mesh now includes face normals
- **Renderer**: Now handles lighting uniforms and material properties
- **Material Design**: Simplified from PBR to Phong for performance

### Performance Notes
- Deliberately kept simple (Phong instead of PBR) for maximum speed
- Single-pass forward rendering
- Optimized for 60+ FPS on integrated graphics
- Lightweight texture management

---

## [0.1.0] - 2025-12-03

### Added - Phase 1 Complete

#### Core Systems
- **Engine Core**: Main engine class with initialization, main loop, and shutdown
- **Time System**: Delta time tracking, FPS counter, time scaling support
- **Logger**: Multi-level console logging system with timestamps
- **Event System**: Pub/sub event dispatcher for decoupled module communication

#### Entity Component System (ECS)
- **World**: Entity and component management
- **Entities**: Lightweight entity IDs with name support
- **Components**: Core components (Transform, Mesh, Camera, Rigidbody, Collider, AIAgent)
- **Systems**: Base system class for game logic processors

#### Rendering
- **OpenGL Renderer**: Basic 3D rendering with depth testing and face culling
- **Shader System**: GLSL shader loading, compilation, and uniform management
- **Camera System**: 3D camera with perspective projection and free-fly controls
- **Mesh System**: Vertex buffer management and basic geometry creation (cube)
- **Material System**: Placeholder for future PBR implementation

#### Window and Input
- **GLFW Integration**: Cross-platform window management
- **Input System**: Keyboard and mouse state tracking with frame-perfect detection
- **Event Callbacks**: Window events (close, resize) integrated with event system

#### Placeholder Systems
- **Physics System**: Structural placeholder for Phase 3 physics integration
- **AI System**: Structural placeholder for Phase 5 AI implementation
  - Documented future features: behavior trees, GOAP, external AI APIs, memory systems

#### Documentation
- **README.md**: Project overview and roadmap
- **docs/ARCHITECTURE.md**: Detailed architecture documentation
- **docs/BUILD.md**: Platform-specific build guides
- **docs/GETTING_STARTED.md**: 10-minute getting started guide
- **docs/ECS_GUIDE.md**: Complete ECS usage guide
- **docs/API_REFERENCE.md**: Quick API reference
- **CONTRIBUTING.md**: Contribution guidelines

#### Demo
- **Phase 1 Demo**: Rotating colored cube with free-fly camera controls
- **Example Scene**: Demonstrates ECS entity creation and component usage

### Technical Details

#### Build System
- CMake 3.15+ build configuration
- Cross-platform support (Windows, Linux, macOS)
- vcpkg integration for Windows
- Asset copying to build directory

#### Dependencies
- C++17 standard
- OpenGL 3.3+ Core Profile
- GLFW 3.x for windowing
- GLM for mathematics
- GLAD for OpenGL loading

#### Architecture Highlights
- Pure ECS architecture for maximum flexibility
- Event-driven module communication
- Separation of concerns (rendering, input, logic)
- Designed for future expansion (physics, AI, networking)

### Developer Experience
- Comprehensive code comments
- Doxygen-style API documentation
- Clear examples in main.cpp
- Extensible system registration

---

## [Unreleased] - Future Phases

### Planned for Phase 2 - Enhanced Rendering
- [ ] Model loading (GLTF, OBJ via Assimp)
- [ ] Texture system with stb_image
- [ ] PBR material system (metallic-roughness workflow)
- [ ] Lighting (directional, point, spot lights)
- [ ] Shadow mapping
- [ ] Skybox and environment maps
- [ ] Normal mapping
- [ ] Render queue with sorting

### Planned for Phase 3 - Physics Integration
- [ ] Rigid body dynamics
- [ ] Collision detection (broad and narrow phase)
- [ ] Constraint solver
- [ ] PhysX or Bullet integration
- [ ] Trigger volumes
- [ ] Raycasting

### Planned for Phase 4 - Advanced Features
- [ ] Particle systems
- [ ] Terrain generation and LOD
- [ ] Audio system (OpenAL)
- [ ] Networking foundation
- [ ] Scene serialization

### Planned for Phase 5 - AI Systems
- [ ] Behavior tree implementation
- [ ] GOAP (Goal-Oriented Action Planning)
- [ ] Sensory system (vision, hearing)
- [ ] External AI API integration (OpenAI, Anthropic)
- [ ] NPC persistent memory with embeddings
- [ ] Pathfinding (A*, Navigation mesh)

### Planned for Phase 6 - Procedural World
- [ ] Voxel/chunk-based world
- [ ] Procedural terrain generation
- [ ] Dynamic world modification
- [ ] World persistence and saving
- [ ] Streaming and LOD

---

## Version History

- **0.1.0** (2025-12-03): Phase 1 - Basic Engine Complete
  - First functional release
  - ECS architecture established
  - 3D rendering with OpenGL
  - Free-fly camera controls
  - Modular, extensible design

---

## Notes

### Breaking Changes
None yet (first release)

### Deprecations
None yet (first release)

### Security
No known security issues

### Migration Guides
N/A (first release)

---

**Current Version**: 0.1.0  
**License**: MIT  
**Repository**: https://github.com/yourusername/nilos-engine

