# Contributing to Nilos Engine

Thank you for your interest in contributing to Nilos Engine! This document provides guidelines and information for contributors.

## Development Setup

### Prerequisites

1. C++17 compatible compiler (MSVC, GCC, Clang)
2. CMake 3.15 or higher
3. Git
4. Dependencies (GLFW, GLM, OpenGL)

### Setting Up Your Development Environment

See [docs/GETTING_STARTED.md](docs/GETTING_STARTED.md) for detailed setup instructions.

```bash
# Quick setup
git clone https://github.com/YOUR_USERNAME/nilos-engine.git
cd nilos-engine
python scripts/setup_glad.py
mkdir build && cd build
cmake ..
cmake --build .
```

## Code Style

### General Guidelines

1. **Code in English**: All code, comments, and documentation must be in English
2. **PascalCase**: Use PascalCase for all code (classes, functions, variables) except Java code
3. **Comments**: Write comprehensive comments explaining the "why" not just the "what"
4. **Documentation**: Use Doxygen-style comments for public APIs

### Example

```cpp
/**
 * @brief Calculate the distance between two points
 * 
 * Uses Euclidean distance formula. For very large distances,
 * consider using Manhattan distance for performance.
 * 
 * @param a First point
 * @param b Second point
 * @return Distance in world units
 */
float CalculateDistance(const glm::vec3& a, const glm::vec3& b) {
    return glm::length(b - a);
}
```

### Formatting

- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line for functions, on new line for classes
- **Line Length**: Prefer 100 characters max, 120 hard limit
- **Includes**: Group by category (standard, third-party, project), sort alphabetically

## Workflow

### 1. Choose an Issue

- Check the [Issues](../../issues) page for open tasks
- Look for issues labeled `good first issue` if you're new
- Comment on the issue to let others know you're working on it

### 2. Create a Branch

```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/issue-description
```

### 3. Make Changes

- Write clean, well-documented code
- Follow the code style guidelines
- Keep commits focused and atomic
- Write descriptive commit messages

### 4. Test Your Changes

```bash
# Build and run
cmake --build .
./bin/NilosEngine

# Verify no warnings
# Check that the engine still runs properly
# Test your specific feature
```

### 5. Commit

```bash
git add .
git commit -m "Add: feature description"
# or
git commit -m "Fix: issue description"
```

**Commit Message Guidelines:**
- Use imperative mood ("Add feature" not "Added feature")
- Keep subject line under 50 characters
- Provide detailed description in body if needed
- Reference issues: "Fixes #123" or "Related to #456"

### 6. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create a pull request on GitHub with:
- Clear title describing the change
- Description of what was changed and why
- Reference to related issues
- Screenshots if applicable (for visual changes)

## What to Contribute

### High Priority

- **Documentation**: Improve README, add tutorials
- **Bug Fixes**: Fix issues from the issue tracker
- **Examples**: Create example projects demonstrating features
- **Tests**: Add unit tests for core systems
- **Platform Support**: Improve cross-platform compatibility

### Medium Priority

- **Performance**: Optimize hot paths, reduce allocations
- **Features**: Implement items from the roadmap
- **Refactoring**: Improve code quality and architecture
- **Tools**: Build tools for developers (mesh viewer, scene editor)

### Future Features (Phase 2+)

- Model loading (GLTF, OBJ)
- Texture system
- Lighting and shadows
- Physics integration
- Audio system
- AI systems

## Areas of Expertise

We need help in various areas:

- **Graphics Programming**: OpenGL, Vulkan, rendering techniques
- **Physics**: Collision detection, rigid body dynamics
- **AI**: Behavior trees, pathfinding, machine learning
- **Networking**: Multiplayer, replication
- **Tools**: Editor development, asset pipeline
- **Documentation**: Technical writing, tutorials

## Code Review Process

1. A maintainer will review your pull request
2. Address any requested changes
3. Once approved, your PR will be merged
4. Your contribution will be acknowledged in the changelog

## Questions?

- **Discord**: (Coming soon)
- **Issues**: For bug reports and feature requests
- **Discussions**: For general questions and ideas

## License

By contributing, you agree that your contributions will be licensed under the same MIT License that covers the project.

## Recognition

Contributors will be:
- Listed in the CONTRIBUTORS.md file
- Mentioned in release notes
- Credited in the about section of the engine

Thank you for helping make Nilos Engine better! 🚀

