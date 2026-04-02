#Requires -RunAsAdministrator

$ErrorActionPreference = "Stop"

$CodeshPath = "C:\Program Files\" + (-join [char[]](0x05E7,0x05D5,0x05D3,0x05E9))

Remove-Item -Recurse -Force $CodeshPath -ErrorAction SilentlyContinue

# Remove CodeshPath from system PATH if present
$syspath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($syspath -like "*$CodeshPath*") {
    $newpath = ($syspath -split ";" | Where-Object { $_ -ne $CodeshPath }) -join ";"
    [Environment]::SetEnvironmentVariable("Path", $newpath, "Machine")
}

Write-Host "Uninstallation complete."
