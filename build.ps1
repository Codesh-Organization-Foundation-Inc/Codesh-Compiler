#Requires -RunAsAdministrator

# AI generated script, sourced from hand-made ./build.sh

$ErrorActionPreference = "Stop"

# Resolve jar from JAVA_HOME
$javaHome = $env:JAVA_HOME
if (-not $javaHome) { $javaHome = [System.Environment]::GetEnvironmentVariable("JAVA_HOME", "Machine") }
if (-not $javaHome) { throw "JAVA_HOME is not set." }
$jar = "$javaHome\bin\jar.exe"
if (-not (Test-Path $jar)) { throw "jar.exe not found at $jar — is a JDK installed?" }

# Resolve cmake from vcpkg's bundled copy if not already in PATH
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    $vcpkgCmake = Get-ChildItem "C:\vcpkg\downloads\tools" -Filter "cmake.exe" -Recurse -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -like "*-windows-x86_64\bin\cmake.exe" } |
        Select-Object -First 1
    if ($vcpkgCmake) {
        $env:PATH = "$($vcpkgCmake.DirectoryName);$env:PATH"
    } else {
        throw "cmake not found. Install CMake or add it to PATH."
    }
}

$CodeshPath = "C:\Program Files\קודש"

# Compile codeshc
cmake -B ./cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
cmake --build ./cmake-build-release

# Deploy to CodeshPath
New-Item -ItemType Directory -Force -Path $CodeshPath | Out-Null
Copy-Item -Force ".\cmake-build-release\codeshc.exe" "$CodeshPath\"

# Add CodeshPath to system PATH if not already present
$syspath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($syspath -notlike "*$CodeshPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$syspath;$CodeshPath", "Machine")
}

# Build Talmud Codesh (use the freshly built binary directly — Hebrew paths can't be invoked by PowerShell)
$TalmudCodeshTemp = "$env:TEMP\talmud-codesh"
New-Item -ItemType Directory -Force -Path $TalmudCodeshTemp | Out-Null
& ".\cmake-build-release\codeshc.exe" --src .\resources\lib-src\ --dest $TalmudCodeshTemp --sinful
if ($LASTEXITCODE -ne 0) { throw "codeshc failed with exit code $LASTEXITCODE" }

# Package as JAR file (jar can't handle Hebrew paths, so build to temp first)
$jarTemp = "$env:TEMP\talmud-codesh.jar"
& $jar cf $jarTemp -C $TalmudCodeshTemp .
Move-Item -Force $jarTemp "$CodeshPath\תלמוד־קודש.jar"

Write-Host "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
