@echo off
echo ========================================
echo   PAC-MAN: World Class Edition
echo   Build Script for Windows
echo ========================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

cd build

echo.
echo Configuring CMake...
echo.

REM Try to use vcpkg if available
if defined VCPKG_ROOT (
    echo Using vcpkg toolchain...
    cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -G "Visual Studio 17 2022" -A x64
) else (
    echo vcpkg not found, using system libraries...
    cmake .. -G "Visual Studio 17 2022" -A x64
)

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo.
    echo Please ensure you have:
    echo   1. CMake installed
    echo   2. Visual Studio 2022 or compatible compiler
    echo   3. GLFW, GLEW, and GLM libraries installed
    echo.
    echo Recommended: Install dependencies using vcpkg:
    echo   vcpkg install glfw3:x64-windows glew:x64-windows glm:x64-windows
    echo.
    pause
    exit /b 1
)

echo.
echo Building project...
echo.

cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo   Build completed successfully!
echo ========================================
echo.
echo Executable location: build\Release\Pacman.exe
echo.
echo To run the game:
echo   cd build\Release
echo   Pacman.exe
echo.
pause
