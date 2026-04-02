#Requires -RunAsAdministrator

$ErrorActionPreference = "Stop"

$CodeshPath = "C:\Program Files\קודש"

# Build using the build script
& "$PSScriptRoot\build.ps1" -OutDir $CodeshPath

# Add CodeshPath to system PATH if not already present
$syspath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($syspath -notlike "*$CodeshPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$syspath;$CodeshPath", "Machine")
}
