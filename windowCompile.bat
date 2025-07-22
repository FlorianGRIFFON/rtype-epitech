@REM bat file that will do some action if build folder is not present

@echo off
setlocal EnableDelayedExpansion

set "folder=build"
set "profileName=default"
set "profileFound=false"

@REM if param "all" is passed, remove build folder
if "%1"=="all" (
    if exist "%folder%" (
        rmdir /s /q "%folder%"
    )
)

if not exist "%folder%" (
    @REM Check if profile exists
    for /f "tokens=*" %%i in ('conan profile list') do (
        set "profile=%%i"
        if "!profileName!"=="!profile!" (
            set "profileFound=true"
            goto :found
        )
    )

    :found
    if %profileFound%==true (
        @REM if profile is found, install dependencies
        conan install . --output-folder=build --build=missing
    ) else (
        @REM if profile is not found, create it and install dependencies
        conan profile detect --force && (
            conan install . --output-folder=build --build=missing
        )
    )

    if %errorlevel% neq 0 (
        echo Error downloading dependencies.
        exit /b %errorlevel%
    )

    @REM Generate project
    cmake -S . -B .\build --preset conan-default

    if %errorlevel% neq 0 (
        echo Error generating project.
        exit /b %errorlevel%
    )
)

if exist "%folder%" (
    @REM Build project
    cmake --build build --config Release

    if %errorlevel% neq 0 (
        echo Error building project.
        exit /b %errorlevel%
    )
)

endlocal
