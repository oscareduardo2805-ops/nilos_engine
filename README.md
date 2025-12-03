# Nilos Engine

A modular 3D graphics engine built with C++17, featuring an Entity Component System (ECS) architecture designed for scalability and extensibility.

## 🚀 Quick Start

```powershell
# Compile
.\build.ps1

# Run
.\run.ps1
```

**First time setup**: See [docs/GETTING_STARTED.md](docs/GETTING_STARTED.md)

---

## Features

- **3D Rendering** - OpenGL 3.3+ with shader support
- **ECS Architecture** - Flexible entity-component-system design
- **Event System** - Decoupled module communication
- **Camera System** - Free-fly camera with keyboard/mouse controls
- **Modular Design** - Easy to extend with new systems (Physics, AI, Networking)

## Controls

- **W/A/S/D** - Move camera
- **Q/E** - Move up/down
- **Right Mouse + Move** - Look around
- **ESC** - Exit

## Documentation

See [docs/](docs/) for complete documentation:
- [Getting Started](docs/GETTING_STARTED.md) - Detailed setup guide
- [Build Instructions](docs/BUILD.md) - Platform-specific build steps
- [Architecture](docs/ARCHITECTURE.md) - System design and patterns
- [ECS Guide](docs/ECS_GUIDE.md) - Entity Component System usage
- [Contributing](docs/CONTRIBUTING.md) - Contribution guidelines

## Project Structure

```
Nilos Engine/
├── src/           # Source code
│   ├── Core/      # Engine core (Logger, Time, Engine)
│   ├── ECS/       # Entity Component System
│   ├── Rendering/ # OpenGL renderer
│   ├── Window/    # GLFW window management
│   └── Input/     # Input system
├── assets/        # Shaders and resources
├── docs/          # Documentation
└── scripts/       # Build and setup scripts
```

## Roadmap

### Phase 1 ✅ (Complete)
- [x] Basic 3D rendering
- [x] ECS architecture
- [x] Camera and input
- [x] Event system

### Phase 2 ✅ (Complete)
- [x] Texture system (stb_image)
- [x] Phong lighting (simple & fast)
- [x] Material system
- [x] Optimized rendering

### Phase 3 ✅ (Complete)
- [x] AABB collision system
- [x] Physics world (gravity, collisions)
- [x] Pathfinding (A* algorithm)
- [x] Model loader foundation

### Phase 4 (Next)
- [ ] AI Behavior Trees
- [ ] Goal-Oriented Action Planning (GOAP)
- [ ] NPC perception system

### Phase 4
- [ ] AI systems (Behavior trees, GOAP)
- [ ] External AI API integration
- [ ] NPC perception and memory

### Phase 6
- [ ] Procedural world generation
- [ ] Voxel-based terrain
- [ ] Dynamic world modification

## Requirements

- **C++17** compiler (MSVC, GCC, Clang)
- **CMake** 3.15+
- **OpenGL** 3.3+
- **GLFW3** and **GLM** libraries

**El script `SETUP_COMPLETO.bat` instalará todas las dependencias automáticamente.**

## License

MIT License - See [LICENSE](LICENSE) for details.

## Version

**Current**: 0.1.0 (Phase 1)  
**Last Updated**: December 2025

---

**Documentation**: [docs/](docs/) | **Contributing**: [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) | **Changelog**: [docs/CHANGELOG.md](docs/CHANGELOG.md)
