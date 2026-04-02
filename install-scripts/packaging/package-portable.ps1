param(
    [Parameter(Mandatory)]
    [string]$BuildDir,
    [Parameter(Mandatory)]
    [string]$OutDir
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path "$BuildDir\codeshc.exe")) {
    Write-Error "Binary not found at $BuildDir\codeshc.exe`nRun build.ps1 first"
    exit 1
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

$StagingDir = Join-Path $env:TEMP "codeshc-portable-staging"
Remove-Item -Recurse -Force $StagingDir -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $StagingDir | Out-Null

try {
    Copy-Item "$BuildDir\codeshc.exe" "$StagingDir\codeshc.exe"
    Get-ChildItem "$BuildDir\*.jar" | Copy-Item -Destination $StagingDir

    # MinGW runtime DLLs
    $MinGWBin = "C:\msys64\ucrt64\bin"
    foreach ($dll in @("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")) {
        $path = Join-Path $MinGWBin $dll
        if (Test-Path $path) {
            Copy-Item $path $StagingDir
        } else {
            Write-Warning "$dll not found at $path, skipping"
        }
    }

    # ICU DLLs (version-agnostic detection)
    foreach ($prefix in @("libicudt", "libicuin", "libicuuc")) {
        $found = Get-ChildItem $MinGWBin -Filter "${prefix}*.dll" | Select-Object -First 1
        if ($found) {
            Copy-Item $found.FullName $StagingDir
        } else {
            Write-Warning "ICU DLL for $prefix not found in $MinGWBin, skipping"
        }
    }

    # vcpkg DLLs (located inside the build directory)
    $VcpkgBin = Join-Path $BuildDir "vcpkg_installed\x64-mingw-dynamic\bin"
    if (Test-Path $VcpkgBin) {
        foreach ($dll in Get-ChildItem $VcpkgBin -Filter "*.dll") {
            Copy-Item $dll.FullName $StagingDir
        }
    } else {
        Write-Warning "vcpkg bin not found at $VcpkgBin, skipping"
    }

    # Bundle install/uninstall scripts
    $ScriptsDir = "$PSScriptRoot\.."
    foreach ($script in @("install-global.ps1", "uninstall-global.ps1")) {
        Copy-Item "$ScriptsDir\$script" "$StagingDir\$script"
    }

    $ZipPath = Join-Path $OutDir "codeshc-portable.zip"
    Compress-Archive -Path "$StagingDir\*" -DestinationPath $ZipPath -Force

    Write-Host "Created $ZipPath successfully"
} finally {
    Remove-Item -Recurse -Force $StagingDir -ErrorAction SilentlyContinue
}
