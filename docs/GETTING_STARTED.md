# Nilos Engine - Quick Start Guide

Get up and running with Nilos Engine in under 10 minutes.

## Prerequisites Checklist

- [ ] C++17 compatible compiler installed
- [ ] CMake 3.15+ installed
- [ ] Git installed
- [ ] GPU with OpenGL 3.3+ support

## 1. Clone the Repository

```bash
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine
```

## 2. Install Dependencies

### Windows (vcpkg)

```powershell
# Install vcpkg if not already
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install glfw3:x64-windows glm:x64-windows
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libglm-dev
```

### macOS

```bash
brew install cmake glfw glm
```

## 3. Setup GLAD

```bash
# Install GLAD generator
pip install glad

# Generate GLAD files
python -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"
```

Alternatively, download pre-generated files from https://glad.dav1d.de/

## 4. Build

### Windows

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### Linux/macOS

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)  # Linux
# or
make -j$(sysctl -n hw.ncpu)  # macOS
```

## 5. Run

### Windows

```powershell
.\bin\Release\NilosEngine.exe
```

### Linux/macOS

```bash
./bin/NilosEngine
```

## Controls

- **W/A/S/D** - Move camera
- **Q/E** - Move up/down
- **Right Mouse Button + Move** - Look around
- **ESC** - Exit

## What You'll See

A rotating colored cube in a 3D scene. You can fly around using the camera controls.

## Next Steps

### Learn the Basics

1. **Read the Architecture**: `ARCHITECTURE.md`
2. **Explore the Code**: Start with `src/main.cpp`
3. **Understand ECS**: Check `src/ECS/` modules

### Try Modifying the Demo

Edit `src/Core/Engine.cpp` in the `SetupDemoScene()` function:

```cpp
// Add a second cube
uint32_t cube2 = m_World->CreateEntity("Cube2");
auto* transform2 = m_World->AddComponent<TransformComponent>(cube2);
transform2->Position = glm::vec3(2.0f, 0.0f, 0.0f);
auto* mesh2 = m_World->AddComponent<MeshComponent>(cube2);
mesh2->CreateCube();
```

Rebuild and run to see two cubes!

### Create Your First Entity

```cpp
// In SetupDemoScene() in Engine.cpp

// Create a custom entity
uint32_t myEntity = m_World->CreateEntity("MyFirstEntity");

// Add transform
auto* transform = m_World->AddComponent<TransformComponent>(myEntity);
transform->Position = glm::vec3(0.0f, 1.0f, -3.0f);
transform->Scale = glm::vec3(0.5f, 0.5f, 0.5f);

// Add mesh
auto* mesh = m_World->AddComponent<MeshComponent>(myEntity);
mesh->CreateCube();

// Make it rotate in Update()
// Add to Engine::Update():
auto* myTransform = m_World->GetComponent<TransformComponent>(myEntity);
if (myTransform) {
    myTransform->Rotation.y += 90.0f * deltaTime;  // 90 degrees per second
}
```

## Common Issues

### GLAD Not Found
- Make sure `external/glad/` contains the GLAD files
- See `external/GLAD_SETUP.md` for detailed instructions

### Black Screen
- Update GPU drivers
- Check console output for errors
- Verify OpenGL 3.3+ support

### Build Errors
- Ensure all dependencies are installed
- Clear build directory: `rm -rf build && mkdir build`
- Check `BUILD_INSTRUCTIONS.md` for detailed help

## Resources

- **Full Build Guide**: `BUILD_INSTRUCTIONS.md`
- **Architecture Docs**: `ARCHITECTURE.md`
- **API Reference**: Coming soon
- **Examples**: Coming in Phase 2

## Get Help

- **Issues**: https://github.com/yourusername/nilos-engine/issues
- **Discussions**: https://github.com/yourusername/nilos-engine/discussions

## What's Next?

Now that you have Nilos Engine running, check out:

1. **README.md** - Full project overview and roadmap
2. **ARCHITECTURE.md** - Deep dive into engine design
3. **CONTRIBUTING.md** - How to contribute to the project

Happy coding! 🚀

