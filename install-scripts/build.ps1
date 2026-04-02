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
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
cmake -B $OutDir -S . -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"
cmake --build $OutDir

# Build Talmud Codesh as JAR (use the freshly built binary directly — Hebrew paths can't be invoked by PowerShell)
# --unholy because we are MAKING the Talmud Codesh and do not rely on it
$JarName = (-join [char[]](0x05EA,0x05DC,0x05DE,0x05D5,0x05D3,0x05BE,0x05E7,0x05D5,0x05D3,0x05E9)) + '.jar'
& (Join-Path $OutDir "codeshc.exe") --src .\resources\lib-src\ --dest (Join-Path $OutDir $JarName) --unholy
if ($LASTEXITCODE -ne 0) { throw "codeshc failed with exit code $LASTEXITCODE" }

Write-Host "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
