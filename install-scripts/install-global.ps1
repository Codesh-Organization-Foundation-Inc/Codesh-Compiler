#Requires -RunAsAdministrator

param(
    [Parameter(Mandatory)]
    [string]$BuildDir
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path "$BuildDir\codeshc.exe")) {
    Write-Error "קוֹדֶשׁ אֵינוֹ נִמְצָא בְּתִיקִיַּת הַיַּעַד"
    exit 1
}

$CodeshPath = "C:\Program Files\" + (-join [char[]](0x05E7,0x05D5,0x05D3,0x05E9))

New-Item -ItemType Directory -Force -Path $CodeshPath | Out-Null
Copy-Item -Force "$BuildDir\*" "$CodeshPath\"
Remove-Item -Force -ErrorAction SilentlyContinue "$CodeshPath\install-global.ps1", "$CodeshPath\install-global.sh"

# Add CodeshPath to system PATH if not already present
$syspath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($syspath -notlike "*$CodeshPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$syspath;$CodeshPath", "Machine")
}

Write-Host "וְיִשְׂמַח ה' כִּי עָבְרָה הַהַתְקָנָה עָבְרָה בְּשָׁלוֹם וַיֹּאמֶר לְיוֹצֵר קַדֵּד וְהַצְלַח לֵאמֹ֑ר:"
