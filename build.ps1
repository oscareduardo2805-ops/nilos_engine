# Nilos Engine - Build Script for Development
# Run from Cursor terminal: .\build.ps1

param(
    [switch]$Clean,
    [switch]$Debug,
    [switch]$Run
)

$ErrorActionPreference = "Stop"

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Nilos Engine - Build Script" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Check GLAD
if (-not (Test-Path "external\glad\include\glad\glad.h")) {
    Write-Host "❌ GLAD not found!" -ForegroundColor Red
    Write-Host "Please setup GLAD first. See: docs\SETUP_GLAD.md" -ForegroundColor Yellow
    Write-Host "Quick: Download from https://glad.dav1d.de/" -ForegroundColor Yellow
    exit 1
}

Write-Host "✅ GLAD found" -ForegroundColor Green

# Check vcpkg
$vcpkgPath = "C:\vcpkg\scripts\buildsystems\vcpkg.cmake"
if (-not (Test-Path $vcpkgPath)) {
    Write-Host "⚠️  vcpkg not found at C:\vcpkg" -ForegroundColor Yellow
    Write-Host "Trying without vcpkg (dependencies must be installed manually)" -ForegroundColor Yellow
    $vcpkgPath = $null
}

# Configuration
$config = if ($Debug) { "Debug" } else { "Release" }
$buildDir = "build"

# Clean build
if ($Clean -and (Test-Path $buildDir)) {
    Write-Host "🧹 Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

# Create build directory
if (-not (Test-Path $buildDir)) {
    Write-Host "📁 Creating build directory..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Configure
Write-Host ""
Write-Host "⚙️  Configuring CMake ($config)..." -ForegroundColor Cyan
Push-Location $buildDir

try {
    if ($vcpkgPath) {
        cmake .. -DCMAKE_TOOLCHAIN_FILE=$vcpkgPath -DCMAKE_BUILD_TYPE=$config
    } else {
        cmake .. -DCMAKE_BUILD_TYPE=$config
    }
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ CMake configuration failed" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "✅ Configuration successful" -ForegroundColor Green
    
    # Build
    Write-Host ""
    Write-Host "🔨 Building project ($config)..." -ForegroundColor Cyan
    cmake --build . --config $config
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "❌ Build failed" -ForegroundColor Red
        exit 1
    }
    
    Write-Host ""
    Write-Host "================================================" -ForegroundColor Green
    Write-Host "✅ Build successful!" -ForegroundColor Green
    Write-Host "================================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "Executable: build\bin\$config\NilosEngine.exe" -ForegroundColor Cyan
    
} finally {
    Pop-Location
}

# Run if requested
if ($Run) {
    Write-Host ""
    Write-Host "🚀 Running Nilos Engine..." -ForegroundColor Cyan
    Write-Host "================================================" -ForegroundColor Cyan
    Write-Host ""
    
    $exePath = "build\bin\$config\NilosEngine.exe"
    if (Test-Path $exePath) {
        & $exePath
    } else {
        Write-Host "❌ Executable not found: $exePath" -ForegroundColor Red
        exit 1
    }
}

Write-Host ""
Write-Host "To run manually:" -ForegroundColor Yellow
Write-Host "  cd build\bin\$config" -ForegroundColor White
Write-Host "  .\NilosEngine.exe" -ForegroundColor White
Write-Host ""
Write-Host "Or use:" -ForegroundColor Yellow
Write-Host "  .\build.ps1 -Run" -ForegroundColor White

