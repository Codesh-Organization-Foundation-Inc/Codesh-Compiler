param(
    [Parameter(Mandatory)]
    [string]$OutDir
)

# AI generated script, sourced from hand-made ./build.sh

$ErrorActionPreference = "Stop"

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

# Compile codeshc
cmake -B ./cmake-build-release -S . -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
cmake --build ./cmake-build-release

# Deploy to OutDir
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
Copy-Item -Force ".\cmake-build-release\codeshc.exe" "$OutDir\"

# Build Talmud Codesh as JAR (use the freshly built binary directly — Hebrew paths can't be invoked by PowerShell)
# --unholy because we are MAKING the Talmud Codesh and do not rely on it
& ".\cmake-build-release\codeshc.exe" --src .\resources\lib-src\ --dest "$OutDir\תלמוד־קודש.jar" --sinful --unholy
if ($LASTEXITCODE -ne 0) { throw "codeshc failed with exit code $LASTEXITCODE" }

Write-Host "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
