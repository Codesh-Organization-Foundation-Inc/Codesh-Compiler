# Building Codesh Compiler on Windows

## Prerequisites

### 1. CLion
Provides the MinGW-w64 toolchain — no separate MinGW install is required for building. However, the MinGW runtime DLLs must be in your PATH at runtime (see [Running the Compiler](#running-the-compiler)).

### 2. JDK (Java 11+)
Required to build the Talmud Codesh standard library JAR.

Set `JAVA_HOME` to your JDK installation:
```powershell
[System.Environment]::SetEnvironmentVariable("JAVA_HOME", "C:\Program Files\Java\jdk-21", "Machine")
```

### 3. vcpkg
Install at `C:\vcpkg`:
```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
[System.Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\vcpkg", "Machine")
```

### 4. Visual Studio Build Tools 2022
Required by vcpkg even when using the MinGW toolchain.

```powershell
winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

After installation, open **Visual Studio Installer → Modify → Individual Components** and install the **Windows 11 SDK**.

### 5. Prebuilt ICU 78.3
ICU cannot be built reliably via vcpkg on Windows. Use the prebuilt MSVC binaries:

1. Download: `https://github.com/unicode-org/icu/releases/download/release-78.3/icu4c-78.3-Win64-MSVC2022.zip`
2. Extract to `C:\icu` — the directory must contain `include\`, `lib64\`, and `bin64\`.

---

## Building

### Option A: Automated (installs system-wide, requires Administrator)

Run in an **Administrator** PowerShell:
```powershell
.\build.ps1
```

This will:
- Compile the release binary with MinGW
- Install `codeshc.exe` to `C:\Program Files\קודש\`
- Add that directory to the system PATH
- Compile and install the Talmud Codesh standard library JAR

### Option B: Manual (CLion / debug builds)

1. Open the project in CLion.
2. Go to **Settings → Build, Execution, Deployment → Toolchains** and set the toolchain to **MinGW**.
3. In the CMake profile, set the toolchain to **MinGW**.
4. Build the `codeshc` target normally.

---

## Running the Compiler

The `codeshc.exe` binary depends on MinGW runtime DLLs. If they are not in PATH you will get exit code `0xC0000139` and the process will crash before executing a single line.

Add the MinGW `bin` directory to your system PATH (adjust if your MSYS2 is elsewhere):
```
C:\msys64\ucrt64\bin
```

### CLion run/debug configurations

In each run configuration, add an environment variable:
```
PATH=C:\msys64\ucrt64\bin;$PATH$
```

A preconfigured run configuration for the HTTP test is provided in `.run/`.

---

## Running Compiler Output on Windows

Codesh compiles to JVM bytecode. Running the output has Windows-specific constraints due to Hebrew class names.

### The problem

Windows passes command-line arguments through the system ANSI code page (CP1252/CP437). The JVM receives garbled characters instead of Hebrew, so this does **not** work on Windows even with `chcp 65001`:

```
java תוכנית          # BROKEN on Windows
```

### Solution: always use a JAR

**Preferred — compile directly to JAR:**

If the compiler produces a `.jar` file, it will contain a `Main-Class` manifest entry. Run it as:
```
java -jar תוכנית.jar
```
No Hebrew class name is ever typed in the terminal.

**Manual wrapping — if you have `.class` files:**

1. Create `MANIFEST.MF`:
   ```
   Manifest-Version: 1.0
   Main-Class: תוכנית
   ```

2. If the Talmud Codesh library is **not** bundled into the output, add a `Class-Path` entry. Hebrew path segments must be percent-encoded:
   ```
   Class-Path: file:///C:/Program%20Files/%D7%A7%D7%95%D7%93%D7%A9/%D7%AA%D7%9C%D7%9E%D7%95%D7%93%D6%BE%D7%A7%D7%95%D7%93%D7%A9.jar
   ```
   (If Talmud Codesh classes are bundled into the JAR, skip this line.)

3. Bundle and run:
   ```
   jar cfm program.jar MANIFEST.MF *.class
   java -jar program.jar
   ```
