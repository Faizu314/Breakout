@echo off
setlocal EnableDelayedExpansion

setlocal
set "CMAKE_COMMAND="%CD%\Vendor\Windows\CMake\bin\cmake.exe""

echo Using CMake command: %CMAKE_COMMAND%

:: Verify CMake works
%CMAKE_COMMAND% --version
if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong with the provided CMake executable.
    pause
    exit /b 1
)

:: CHECK VISUAL STUDIO INSTALLATION

set BUILD_COMMAND=%CMAKE_COMMAND% --build . --config Release

set VS_PATH_2022_X86=1C:\Program Files (x86)\Microsoft Visual Studio\2022
set VS_PATH_2022_X64=1C:\Program Files\Microsoft Visual Studio\2022
set VS_PATH_2019_X86=C:\Program Files (x86)\Microsoft Visual Studio\2019
set VS_PATH_2019_X64=C:\Program Files\Microsoft Visual Studio\2019
set VS_PATH_2017_X86=C:\Program Files (x86)\Microsoft Visual Studio\2017
set VS_PATH_2017_X64=C:\Program Files\Microsoft Visual Studio\2017

echo Checking if Visual Studio exists at the following locations:
echo %VS_PATH_2022_X86%\Community
echo %VS_PATH_2022_X64%\Community
echo %VS_PATH_2019_X86%\Community
echo %VS_PATH_2019_X64%\Community
echo %VS_PATH_2017_X86%\Community
echo %VS_PATH_2017_X64%\Community

if exist "%VS_PATH_2022_X86%\Community" (
    set GENERATOR=Visual Studio 17 2022
) else if exist "%VS_PATH_2022_X64%\Community" (
    set GENERATOR=Visual Studio 17 2022
) else if exist "%VS_PATH_2019_X86%\Community" (
    set GENERATOR=Visual Studio 16 2019
) else if exist "%VS_PATH_2019_X64%\Community" (
    set GENERATOR=Visual Studio 16 2019
) else if exist "%VS_PATH_2017_X86%\Community" (
    set GENERATOR=Visual Studio 15 2017
) else if exist "%VS_PATH_2017_X64%\Community" (
    set GENERATOR=Visual Studio 15 2017
) else (
    echo No compatible Visual Studio version found, falling back to Ninja Generator
    set GENERATOR=Ninja
    call :get_ninja_command
)

if not exist "Build" (
    mkdir "Build"
)
cd "Build"

if "!GENERATOR!" == "Ninja" (
    echo Using Ninja Generator...
    !CMAKE_COMMAND! .. -G "!GENERATOR!" -DCMAKE_MAKE_PROGRAM="!NINJA_EXECUTABLE!"

    if %ERRORLEVEL% neq 0 (
        echo Build failed. Please check the errors above.
        pause
        exit /b 1
    )
) else (
    echo Using generator: !GENERATOR!
    !CMAKE_COMMAND! .. -G "!GENERATOR!"

    if %ERRORLEVEL% neq 0 (
        echo Failed to generate visual studio files, falling back to ninja.
        set GENERATOR=Ninja
        call :get_ninja_command

        echo Using Ninja Generator...
        !CMAKE_COMMAND! .. -G "!GENERATOR!" -DCMAKE_MAKE_PROGRAM="!NINJA_EXECUTABLE!"

        if %ERRORLEVEL% neq 0 (
            echo Build failed. Please check the errors above.
            pause
            exit /b 1
        )
    )
)

%BUILD_COMMAND%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit /b 1
)

set /p USER_INPUT="Do you want to run Breakout? You will find exe at /Build/Release/Game.exe (Y/N)? "

if /i "%USER_INPUT%"=="Y" (
    echo Running Game.exe...
    start Release\Game.exe
) else (
    echo Exiting. Game not launched.
)

pause
exit /b 0



:get_ninja_command

echo Checking for Ninja...
where ninja >nul 2>&1
if %ERRORLEVEL% neq 0 (
    set "NINJA_DIR=%CD%\Vendor\Windows\ninja"

    if exist "!NINJA_DIR!\ninja.exe" (
        echo Downloaded Ninja exe already exists...
        set "NINJA_EXECUTABLE=!NINJA_DIR!\ninja.exe"
        exit /b 0
    )

    echo Ninja not found, attempting to download ninja...

    set "NINJA_URL=https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-win.zip"
    set "ZIP_PATH=%CD%\ninja-win.zip"

    if not exist "!ZIP_PATH!" (
        echo Downloading Ninja pre-compiled binary...
        echo powershell -Command "(New-Object System.Net.WebClient).DownloadFile('!NINJA_URL!', '!ZIP_PATH!')"
        powershell -Command "(New-Object System.Net.WebClient).DownloadFile('!NINJA_URL!', '!ZIP_PATH!')"

        if exist "!ZIP_PATH!" (
            echo Ninja archive downloaded successfully.
        ) else (
            echo Ninja download failed!
            pause
            exit 1
        )
    )

    echo Expanding downloaded ninja zip file...

    if not exist "!NINJA_DIR!" (
        mkdir "!NINJA_DIR!"
    )
    powershell -Command "Expand-Archive '!ZIP_PATH!' -DestinationPath '!NINJA_DIR!'"

    set "NINJA_EXECUTABLE=!NINJA_DIR!\ninja.exe"

    if exist "!NINJA_EXECUTABLE!" (
        echo Ninja zip file expanded successfully, deleting zip file
        del "!ZIP_PATH!"
    )

    echo Ninja executable is located at: !NINJA_EXECUTABLE!
) else (
    echo Found Ninja at:
    for /f "tokens=*" %%i in ('where ninja') do set "NINJA_EXECUTABLE=%%i"
    echo !NINJA_EXECUTABLE!
)

exit /b
endlocal
