# Nilos Engine - Quick Run Script
# Run from Cursor terminal: .\run.ps1

param(
    [switch]$Debug
)

$config = if ($Debug) { "Debug" } else { "Release" }
$exePath = "build\bin\$config\NilosEngine.exe"

Write-Host "================================================" -ForegroundColor Cyan
Write-Host "Nilos Engine - Run ($config)" -ForegroundColor Cyan
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Check if built
if (-not (Test-Path $exePath)) {
    Write-Host "❌ Executable not found!" -ForegroundColor Red
    Write-Host "Build first with: .\build.ps1" -ForegroundColor Yellow
    exit 1
}

# Run
Write-Host "🚀 Starting engine..." -ForegroundColor Green
Write-Host ""
Write-Host "Controls:" -ForegroundColor Cyan
Write-Host "  W/A/S/D - Move camera" -ForegroundColor White
Write-Host "  Q/E     - Up/Down" -ForegroundColor White
Write-Host "  RMB+Move - Look around" -ForegroundColor White
Write-Host "  ESC     - Exit" -ForegroundColor White
Write-Host ""
Write-Host "================================================" -ForegroundColor Cyan
Write-Host ""

# Run and capture exit code
& $exePath
$exitCode = $LASTEXITCODE

Write-Host ""
Write-Host "================================================" -ForegroundColor Cyan
if ($exitCode -eq 0) {
    Write-Host "Engine exited normally" -ForegroundColor Green
} else {
    Write-Host "Engine exited with code: $exitCode" -ForegroundColor Yellow
}
Write-Host "================================================" -ForegroundColor Cyan

