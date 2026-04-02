# Building Codesh Compiler on Windows (why)

Do you feel like being a masochist for the day?  
Do you not value your time at all?  
Do you *really* hate your life and mere being with every fiber of your miserable soul?

If so, I welcome you with open hands to the best guide on the internet for unemployed individuals like yourself.

P.S: Linux is completely free

## Before we Begin

You can avoid all the below by one of 2 ways:

### Quick Install

1. Install the latest realease
2. Enjoy

### Use Linux

Much better

###  Building with **WSL** *(Windows Subsystem for Linux)*

A simple PowerShell command for the linuxphobes:

```powershell
wsl --install fedora
```

*(The Motzi B'She'ela is heavily tested on Fedora and its official build guides are for Fedora's DNF. Other distros work too.)*

From there you may follow the Linux build guide (1 minute read).

---

If you really want to suffer natively on Windows, continue below.

Make sure to have the rest of your 3 hours of your time free.

This guide is not responsible for any mental damage caused by following the instructions below.

---


## Prerequisites

### 1. MinGW-w64 (via MSYS2)
Install MSYS2 from [msys2.org](https://www.msys2.org/), then install the MinGW-w64 toolchain:

```powershell
winget install MSYS2.MSYS2
```

Then open the **MSYS2 UCRT64** shell and run:
```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make
```

Add the MinGW bin directory to your system PATH:
```
C:\msys64\ucrt64\bin
```

The MinGW runtime DLLs must be in your PATH **at runtime** (see [Running the Compiler](#running-the-compiler)).

### 2. JDK (Java 11+) (preferably 21)
Required to build the Talmud Codesh standard library JAR.

Set the `JAVA_HOME` environment variable to that of your JDK installation.

### 3. vcpkg
Install at `C:\vcpkg`:
```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Add to PATH
[System.Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\vcpkg", "Machine")
```

### 4. Visual Studio Build Tools 2022
Required by vcpkg. Yes Patrick, even when using the MinGW toolchain. No Patrick, you don't need to do this if you have Visual Studio already installed.

```powershell
winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

After installation, open the **Visual Studio Installer → Modify → Individual Components** and install the **Windows 11 SDK**.

### 5. Using the Prebuilt ICU
ICU cannot be properly built via vcpkg on Windows due to incompatibilities between the MSVC and MinGW builds.

Use the prebuilt MSVC binaries instead:

1. Download: `https://github.com/unicode-org/icu/releases/download/release-78.3/icu4c-78.3-Win64-MSVC2022.zip`
2. Extract to `C:\icu`.

## Building

### Option A: Automated (installs system-wide, requires Administrator)

Run this in an **Administrator** PowerShell:
```powershell
.\build.ps1
```

This will:
- Compile the release binary with MinGW
- Install `codeshc.exe` to `C:\Program Files\קודש\`
- Add that directory to the system PATH
- Compile and install Talmud Codesh as a JAR

### Option B: Manual (debug builds)

Run in a PowerShell with `C:\msys64\ucrt64\bin` in your PATH:

```powershell
cmake -B ./cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
cmake --build ./cmake-build-debug
```

The binary will be at `.\cmake-build-debug\codeshc.exe`.

### Option C: Lion (get it because C, it's C-LION, so CLion) (CLion)

1. Open the project in CLion.
2. Go to **Settings → Build, Execution, Deployment → Toolchains** and add a **MinGW** toolchain pointing to `C:\msys64\ucrt64`.
3. In the CMake profile, set the toolchain to **MinGW**.
4. Build the `codeshc` target normally.
5. Psych bitch, you still need the next section. <sub>sorry</sub>

## Running the Compiler

The `codeshc.exe` binary **depends on MinGW runtime DLLs**.

Add the MinGW `bin` directory to your system PATH (adjust if your MSYS2 is elsewhere):
```
C:\msys64\ucrt64\bin
```

Alternatively, you may copy `libgcc_s_seh-1.dll`, `libstdc++-6.dll`, and `libwinpthread-1.dll` from the above directory to the same directory as `codeshc.exe`.

### Running from PowerShell

Prepend the MinGW bin directory to your session PATH before invoking the compiler:

```powershell
$env:PATH = "C:\msys64\ucrt64\bin;$env:PATH"
.\cmake-build-debug\codeshc.exe <source_path> <output_path>
```

# Conclusions

1. I had to go through this shit myself
2. You did not have to go through this shit yourself
3. Fuck Windows
4. Fuck me
