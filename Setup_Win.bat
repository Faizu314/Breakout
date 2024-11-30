@echo off

if not exist "Build"  (
	mkdir "Build"
)
cd "Build"

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
    goto :found
)

if exist "%VS_PATH_2022_X64%\Community" (
    set GENERATOR=Visual Studio 17 2022
    goto :found
)

if exist "%VS_PATH_2019_X86%\Community" (
    set GENERATOR=Visual Studio 16 2019
    goto :found
)

if exist "%VS_PATH_2019_X64%\Community" (
    set GENERATOR=Visual Studio 16 2019
    goto :found
)

if exist "%VS_PATH_2017_X86%\Community" (
    set GENERATOR=Visual Studio 15 2017
    goto :found
)

if exist "%VS_PATH_2017_X64%\Community" (
    set GENERATOR=Visual Studio 15 2017
    goto :found
)

echo No compatible Visual Studio version found.
pause
exit /b 1

:found
echo Using generator: %GENERATOR%
cmake .. -G "%GENERATOR%"

cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo Build failed. Please check the errors above.
    pause
    exit /b 1
)

set /p USER_INPUT="Do you want to run Breakout. You will find exe at /Build/Release/Game.exe (Y/N)? "

if /i "%USER_INPUT%"=="Y" (
    echo Running Game.exe...
    start Release\Game.exe
) else (
    echo Exiting. Game not launched.
)

pause