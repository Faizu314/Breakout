#!/bin/bash

if ! xcode-select -p &>/dev/null; then
    echo "Xcode Command Line Tools not detected. Installing..."

    xcode-select --install

    echo "Please complete the installation through the prompt. The script will wait until installation is complete."

    while ! xcode-select -p &>/dev/null; do
        sleep 5
    done

    echo "Xcode Command Line Tools installed successfully."
else
    echo "Xcode Command Line Tools are already installed."
fi

CmakePath=""

if command -v cmake &>/dev/null; then
    echo "CMake is already installed: $(cmake --version)"
    CmakePath="cmake"
elif [ -f "$(pwd)/CMake/bin/cmake" ]; then
    echo "CMake found in the local directory: $(pwd)/CMake/bin/cmake"
    CmakePath="$(pwd)/CMake/bin/cmake"
else
    echo "CMake not found. Proceeding to download... (script will clone inside directory and not install on the machine)"
    git clone https://github.com/Kitware/CMake.git
    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone the CMake repository. Please check your internet connection and try again."
        exit 1
    fi
    
    cd CMake
    ./bootstrap
    if [ $? -ne 0 ]; then
        echo "Error: Cmake Bootstrap failed. Exiting."
        exit 1
    fi

    NUM_CORES=$(sysctl -n hw.ncpu)
    echo "Detected $NUM_CORES cores. Using them for parallel build."
    make -j"$NUM_CORES"

    cd ..
    CmakePath="$(pwd)/CMake/bin/cmake"
    echo "CMake has been built locally. Using CMake at: $CmakePath"
fi

echo "Environment setup complete! Proceeding to build engine and game..."

mkdir -p Build
cd Build || exit

if [ -d "/Applications/Xcode.app" ]; then
    echo "Xcode IDE detected. Using Xcode generator..."

    if "$CmakePath" .. -G "Xcode"; then
        echo "Xcode generator succeeded. Proceeding with build."
        xcodebuild -configuration Release
        executable_path="Release/Game.app"
    else
        echo "Xcode generator failed. Falling back to Unix Makefiles generator..."
        "$CmakePath" .. -G "Unix Makefiles"
        make
        executable_path="Game.app"
    fi
else
    echo "Xcode IDE not detected. Using Unix Makefiles generator..."

    "$CmakePath" .. -G "Unix Makefiles"
    make
    executable_path="Game.app"
fi

read -p "Do you want to run the Breakout? You will find the app bundle at Build/$executable_path (Y/N): " user_input

user_input=$(echo "$user_input" | tr '[:upper:]' '[:lower:]')

if [[ "$user_input" == "y" ]]; then
    echo "Running Breakout game..."
    open "$executable_path"
else
    echo "Exiting. Game not launched."
fi
