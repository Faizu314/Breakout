#!/bin/bash

mkdir -p Build
cd Build || exit

cmake .. -G "Xcode"
xcodebuild -configuration Release

read -p "Do you want to run the Breakout? You will find the app bundle at /Build/Release/Game.app (Y/N): " user_input

user_input=$(echo "$user_input" | tr '[:upper:]' '[:lower:]')

if [[ "$user_input" == "y" ]]; then
    echo "Running Breakout game..."
    open Release/Game.app
else
    echo "Exiting. Game not launched."
fi