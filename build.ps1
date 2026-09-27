$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Set-Location "build"

cmake ..
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build . --config Release
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Set-Location $ScriptDir
