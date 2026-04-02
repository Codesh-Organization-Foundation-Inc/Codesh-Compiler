param(
    [Parameter(Mandatory)]
    [string]$Binary,
    [Parameter(Mandatory)]
    [string]$OutDir
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Binary)) {
    Write-Error "Binary not found at $Binary`nRun build.ps1 first"
    exit 1
}

Remove-Item -Recurse -Force $OutDir -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

Copy-Item $Binary "$OutDir\codeshc.exe"

# MinGW runtime DLLs
$MinGWBin = "C:\msys64\ucrt64\bin"
foreach ($dll in @("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")) {
    $path = Join-Path $MinGWBin $dll
    if (Test-Path $path) {
        Copy-Item $path $OutDir
    } else {
        Write-Warning "$dll not found at $path, skipping"
    }
}

# ICU DLLs (version-agnostic detection)
foreach ($prefix in @("libicudt", "libicuin", "libicuuc")) {
    $found = Get-ChildItem $MinGWBin -Filter "${prefix}*.dll" | Select-Object -First 1
    if ($found) {
        Copy-Item $found.FullName $OutDir
    } else {
        Write-Warning "ICU DLL for $prefix not found in $MinGWBin, skipping"
    }
}

# vcpkg DLLs (located inside the build directory)
$BuildDir = Split-Path $Binary -Parent
$VcpkgBin = Join-Path $BuildDir "vcpkg_installed\x64-mingw-dynamic\bin"
if (Test-Path $VcpkgBin) {
    foreach ($dll in Get-ChildItem $VcpkgBin -Filter "*.dll") {
        Copy-Item $dll.FullName $OutDir
    }
} else {
    Write-Warning "vcpkg bin not found at $VcpkgBin, skipping"
}

# Bundle install script
Copy-Item "$PSScriptRoot\install-global.ps1" "$OutDir\install-global.ps1"

Compress-Archive -Path "$OutDir\*" -DestinationPath "codeshc-portable.zip" -Force

Write-Host "Created codeshc-portable.zip successfully"
