@echo off
REM Build script for Nilos Engine (Windows)

echo ================================================
echo Nilos Engine - Build Script (Windows)
echo ================================================
echo.

REM Check if GLAD is set up
if not exist "external\glad\include\glad\glad.h" (
    echo WARNING: GLAD not found. Running setup...
    echo.
    python scripts\setup_glad.py
    if errorlevel 1 (
        echo.
        echo ERROR: GLAD setup failed. Please set up GLAD manually.
        echo See docs\SETUP_GLAD.md for instructions.
        pause
        exit /b 1
    )
    echo.
)

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
echo.

REM Try to use vcpkg if available
if defined VCPKG_ROOT (
    cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
) else if exist "C:\vcpkg\scripts\buildsystems\vcpkg.cmake" (
    cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
) else (
    echo WARNING: vcpkg not found. Make sure GLFW and GLM are installed.
    cmake ..
)

if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed.
    cd ..
    pause
    exit /b 1
)

echo.
echo Building...
echo.

REM Build
cmake --build . --config Release

if errorlevel 1 (
    echo.
    echo ERROR: Build failed.
    cd ..
    pause
    exit /b 1
)

echo.
echo ================================================
echo Build successful!
echo ================================================
echo.
echo Executable: build\bin\Release\NilosEngine.exe
echo.
echo To run:
echo   cd build\bin\Release
echo   NilosEngine.exe
echo.

cd ..
pause

