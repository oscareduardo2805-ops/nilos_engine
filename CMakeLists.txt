cmake_minimum_required(VERSION 3.15)
project(NilosEngine VERSION 0.1.0 LANGUAGES C CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(NILOS_BUILD_EXAMPLES "Build example applications" ON)
option(NILOS_USE_VULKAN "Use Vulkan instead of OpenGL" OFF)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Source files
file(GLOB_RECURSE NILOS_SOURCES 
    "src/Core/*.cpp"
    "src/Rendering/*.cpp"
    "src/Window/*.cpp"
    "src/Input/*.cpp"
    "src/Physics/*.cpp"
    "src/AI/*.cpp"
)

file(GLOB_RECURSE NILOS_HEADERS 
    "src/Core/*.h"
    "src/ECS/*.h"
    "src/Events/*.h"
    "src/Rendering/*.h"
    "src/Window/*.h"
    "src/Input/*.h"
    "src/Physics/*.h"
    "src/AI/*.h"
    "include/*.h"
)

# GLAD - OpenGL loader (we'll use a header-only version)
set(GLAD_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c")
set(GLAD_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include")

# Create engine library
add_library(NilosEngineLib STATIC ${NILOS_SOURCES} ${NILOS_HEADERS} ${GLAD_SOURCES})

target_include_directories(NilosEngineLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLAD_INCLUDE}
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(NilosEngineLib PUBLIC
    OpenGL::GL
    glfw
    glm::glm
)

# Compiler warnings
if(MSVC)
    target_compile_options(NilosEngineLib PRIVATE /W4)
else()
    target_compile_options(NilosEngineLib PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Main executable
add_executable(NilosEngine src/main.cpp)
target_link_libraries(NilosEngine PRIVATE NilosEngineLib)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Print configuration
message(STATUS "Nilos Engine Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Use Vulkan: ${NILOS_USE_VULKAN}")

