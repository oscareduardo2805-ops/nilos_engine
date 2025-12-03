param([switch]$Clean, [switch]$Debug, [switch]$Run)

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Nilos Engine - Build Script" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Clean if requested
if ($Clean -and (Test-Path build)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force build
    Write-Host "Done!" -ForegroundColor Green
    Write-Host ""
}

# Create build directory
if (-not (Test-Path build)) {
    mkdir build | Out-Null
}

# Determine configuration
$cfg = if ($Debug) { "Debug" } else { "Release" }

# Configure and build
Push-Location build

Write-Host "Configuring CMake..." -ForegroundColor Cyan
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

if ($LASTEXITCODE -ne 0) {
    Pop-Location
    Write-Host ""
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Building ($cfg)..." -ForegroundColor Cyan
cmake --build . --config $cfg

if ($LASTEXITCODE -ne 0) {
    Pop-Location
    Write-Host ""
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Pop-Location

Write-Host ""
Write-Host "================================================" -ForegroundColor Green
Write-Host "Build successful!" -ForegroundColor Green
Write-Host "================================================" -ForegroundColor Green
Write-Host ""
Write-Host "Executable: build\bin\$cfg\NilosEngine.exe" -ForegroundColor Cyan

# Run if requested
if ($Run) {
    Write-Host ""
    Write-Host "Running Nilos Engine..." -ForegroundColor Cyan
    Write-Host ""
    & "build\bin\$cfg\NilosEngine.exe"
}

if (-not $Run) {
    Write-Host ""
    Write-Host "To run: .\run.ps1 or .\build.ps1 -Run" -ForegroundColor Yellow
}
