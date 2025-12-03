#!/bin/bash
# Build script for Nilos Engine (Linux/macOS)

set -e  # Exit on error

echo "================================================"
echo "Nilos Engine - Build Script (Linux/macOS)"
echo "================================================"
echo

# Check if GLAD is set up
if [ ! -f "external/glad/include/glad/glad.h" ]; then
    echo "⚠️  GLAD not found. Running setup..."
    echo
    python3 scripts/setup_glad.py
    if [ $? -ne 0 ]; then
        echo
        echo "❌ GLAD setup failed. Please set up GLAD manually."
        echo "See docs/SETUP_GLAD.md for instructions."
        exit 1
    fi
    echo
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
echo
cmake ..

# Build
echo
echo "Building..."
echo
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo
echo "================================================"
echo "✅ Build successful!"
echo "================================================"
echo
echo "Executable: build/bin/NilosEngine"
echo
echo "To run:"
echo "  cd build/bin"
echo "  ./NilosEngine"
echo

