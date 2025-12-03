# Detailed Build Instructions

This document provides step-by-step instructions for building Nilos Engine on different platforms.

## Table of Contents

- [Windows](#windows)
- [Linux](#linux)
- [macOS](#macos)
- [Troubleshooting](#troubleshooting)

---

## Windows

### Prerequisites

1. **Visual Studio 2019 or later** (Community Edition is free)
   - Download: https://visualstudio.microsoft.com/
   - Install "Desktop development with C++" workload
   - Includes CMake and MSVC compiler

2. **vcpkg** (for dependency management)
   ```powershell
   # Open PowerShell as Administrator
   git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
   cd C:\vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```

3. **Install Dependencies**
   ```powershell
   cd C:\vcpkg
   .\vcpkg install glfw3:x64-windows glm:x64-windows
   ```

4. **GLAD Setup**
   - Follow instructions in `external/GLAD_SETUP.md`
   - Generate GLAD files for OpenGL 3.3 Core
   - Place in `external/glad/` directory

### Building with Visual Studio

```powershell
# Clone the repository
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine

# Create build directory
mkdir build
cd build

# Generate Visual Studio solution
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# Build (or open the .sln file in Visual Studio)
cmake --build . --config Release
```

### Building with CMake + Ninja (faster)

```powershell
# Install Ninja
C:\vcpkg\vcpkg install ninja:x64-windows

# Configure and build
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

### Running

```powershell
# From build directory
.\bin\Release\NilosEngine.exe
```

---

## Linux

### Ubuntu/Debian

```bash
# Update package list
sudo apt update

# Install compiler and build tools
sudo apt install build-essential cmake git

# Install dependencies
sudo apt install libglfw3-dev libglm-dev

# Clone repository
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine

# Setup GLAD (follow external/GLAD_SETUP.md)
# Quick setup:
pip install glad
python -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./bin/NilosEngine
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel cmake glfw glm

# Clone and build
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine

# Setup GLAD
pip install glad
python -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./bin/NilosEngine
```

### Fedora

```bash
# Install dependencies
sudo dnf install gcc-c++ cmake glfw-devel glm-devel

# Clone and build
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine

# Setup GLAD
pip install glad
python -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run
./bin/NilosEngine
```

---

## macOS

### Prerequisites

1. **Xcode Command Line Tools**
   ```bash
   xcode-select --install
   ```

2. **Homebrew** (package manager)
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **Install Dependencies**
   ```bash
   brew install cmake glfw glm
   ```

### Building

```bash
# Clone repository
git clone https://github.com/yourusername/nilos-engine.git
cd nilos-engine

# Setup GLAD
pip3 install glad
python3 -m glad --generator c --spec gl --out-path external/glad --api="gl=3.3"

# Build
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Run
./bin/NilosEngine
```

---

## CMake Build Options

### Standard Options

```bash
# Debug build (with debug symbols)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release build (optimized)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

### Custom Options

```bash
# Use Vulkan instead of OpenGL (future)
cmake .. -DNILOS_USE_VULKAN=ON

# Build examples (when available)
cmake .. -DNILOS_BUILD_EXAMPLES=ON
```

### Specify Compiler

```bash
# GCC
cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

# Clang
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

---

## Troubleshooting

### GLAD Not Found

**Error**: `fatal error: glad/glad.h: No such file or directory`

**Solution**:
1. Check that `external/glad/include/glad/glad.h` exists
2. Regenerate GLAD files (see `external/GLAD_SETUP.md`)
3. Clear CMake cache: `rm -rf build && mkdir build`

### GLFW Not Found

**Error**: `Could NOT find glfw3 (missing: glfw3_DIR)`

**Solution**:
- **Windows**: Install via vcpkg: `vcpkg install glfw3:x64-windows`
- **Linux**: Install dev package: `sudo apt install libglfw3-dev`
- **macOS**: Install via Homebrew: `brew install glfw`

### GLM Not Found

**Error**: `Could NOT find glm (missing: glm_DIR)`

**Solution**:
- **Windows**: Install via vcpkg: `vcpkg install glm:x64-windows`
- **Linux**: Install dev package: `sudo apt install libglm-dev`
- **macOS**: Install via Homebrew: `brew install glm`

### OpenGL Not Found (Linux)

**Error**: `Could NOT find OpenGL`

**Solution**:
```bash
# Install OpenGL development files
sudo apt install libgl1-mesa-dev
```

### Linking Errors on Linux

**Error**: `undefined reference to 'glfwInit'`

**Solution**:
```bash
# Install GLFW runtime libraries
sudo apt install libglfw3
```

### macOS OpenGL Deprecation Warnings

**Warning**: OpenGL is deprecated on macOS

**Note**: This is expected. OpenGL still works on current macOS versions. Future versions will use Metal/MoltenVK.

### Black Screen on Launch

**Possible Causes**:
1. **Outdated GPU drivers**: Update your graphics drivers
2. **OpenGL version**: Ensure your GPU supports OpenGL 3.3
3. **GLAD not initialized**: Check console output for errors

### Permission Denied (Linux/macOS)

**Error**: `bash: ./NilosEngine: Permission denied`

**Solution**:
```bash
chmod +x ./bin/NilosEngine
./bin/NilosEngine
```

### CMake Version Too Old

**Error**: `CMake 3.15 or higher is required`

**Solution**:
- **Ubuntu**: Add Kitware repository for latest CMake
  ```bash
  wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
  sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
  sudo apt update
  sudo apt install cmake
  ```
- **macOS**: `brew upgrade cmake`
- **Windows**: Download from https://cmake.org/download/

---

## IDE Integration

### Visual Studio Code

1. Install extensions:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)

2. Open project folder in VS Code

3. Configure CMake:
   - Ctrl+Shift+P → "CMake: Configure"
   - Select compiler kit

4. Build:
   - Ctrl+Shift+P → "CMake: Build"
   - Or click "Build" in status bar

5. Debug:
   - Set breakpoints
   - F5 to start debugging

### Visual Studio (Windows)

1. Open Visual Studio
2. File → Open → CMake...
3. Select `CMakeLists.txt`
4. Visual Studio will configure automatically
5. Build: Ctrl+Shift+B
6. Debug: F5

### CLion

1. Open project (select `CMakeLists.txt`)
2. CLion will configure automatically
3. Build: Ctrl+F9
4. Run: Shift+F10
5. Debug: Shift+F9

---

## Building for Distribution

### Windows

```powershell
# Release build with optimizations
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Create installer (when available)
cpack -G NSIS
```

### Linux

```bash
# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Create package
cpack -G DEB  # Debian/Ubuntu
cpack -G RPM  # Fedora/RHEL
```

### macOS

```bash
# Release build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)

# Create app bundle
cpack -G DragNDrop
```

---

## Next Steps

After successful build:
1. Read `README.md` for usage instructions
2. Check `ARCHITECTURE.md` to understand the codebase
3. Explore example code in `src/main.cpp`
4. See `CONTRIBUTING.md` if you want to contribute

For issues, check the [GitHub Issues](https://github.com/yourusername/nilos-engine/issues) page.

