#Requires -RunAsAdministrator

$ErrorActionPreference = "Stop"

$CodeshPath = "C:\Program Files\קודש"

Remove-Item -Force "C:\Program Files\קודש\codeshc.exe" -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force $CodeshPath -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force ".\cmake-build-release" -ErrorAction SilentlyContinue

# Remove CodeshPath from system PATH if present
$syspath = [Environment]::GetEnvironmentVariable("Path", "Machine")
if ($syspath -like "*$CodeshPath*") {
    $newpath = ($syspath -split ";" | Where-Object { $_ -ne $CodeshPath }) -join ";"
    [Environment]::SetEnvironmentVariable("Path", $newpath, "Machine")
}

Write-Host "וְיִגְעַר ה' בַּיּוֹצֵר כִּי־בָּחַר בַּחֹשֶׁךְ וְלֹא בָאוֹר וַיִּתְעַצֵּב מְאֹד מְאֹד וַיִּתֹּם:"
