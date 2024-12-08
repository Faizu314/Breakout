@echo off

set "CMAKE_COMMAND="%CD%\Vendor\Windows\CMake\bin\cmake.exe""

echo Using CMake command: %CMAKE_COMMAND%

%CMAKE_COMMAND% --version
if %ERRORLEVEL% neq 0 (
    echo Error: Something went wrong with the provided CMake executable.
    pause
    exit 1
)

:: CHECK VISUAL STUDIO INSTALLATION

set BUILD_COMMAND=%CMAKE_COMMAND% --build . --config Release

set VS_PATH_2022_X86=C:\Program Files (x86)\Microsoft Visual Studio\2022
set VS_PATH_2022_X64=C:\Program Files\Microsoft Visual Studio\2022
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
    echo No compatible Visual Studio version found, exiting.
    pause
    exit 1
)

if not exist "Build" (
    mkdir "Build"
)
cd "Build"

echo Using generator: %GENERATOR%
%CMAKE_COMMAND% .. -G "%GENERATOR%"

if %ERRORLEVEL% neq 0 (
    echo Failed to generate visual studio files.

    pause
    exit 1
)

%BUILD_COMMAND%

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit 1
)

set /p USER_INPUT="Do you want to run Breakout? You will find exe at /Build/Release/Game.exe (Y/N)? "

if /i "%USER_INPUT%"=="Y" (
    echo Running Game.exe...
    start Release\Game.exe
) else (
    echo Exiting. Game not launched.
)

pause
exit 0