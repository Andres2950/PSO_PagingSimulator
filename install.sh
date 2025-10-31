#!/bin/bash
set -e

# Get the current user and directory
user=$(logname)
DIR=$(pwd)

# Check for root permissions
if [ "$EUID" -ne 0 ]; then
  echo "This script requires root permissions."
  echo "It will be executed again with su -c"
  exec su -c "$0"
fi

# Install g++ if not installed
install_gplusplus() {
  if command -v g++ &>/dev/null; then
    echo "G++ is already installed. Version: $(g++ --version | head -n1)"
  else
    echo "Installing G++..."
    if command -v apt &>/dev/null; then
      apt update
      apt install -y g++
    fi
  fi
}

# Install cmake if not installed
install_cmake() {
  if command -v cmake &>/dev/null; then
    echo "CMake is already installed. Version: $(cmake --version | head -n1)"
  else
    echo "Installing CMake..."
    if command -v apt &>/dev/null; then
      apt update
      apt install -y cmake
    fi
  fi
}

# Install SDL dependencies
install_sdl_dependencies() {
  echo "Installing SDL dependencies..."
  apt install -y libsdl3-dev
}

# installation
install_gplusplus
install_cmake
install_sdl_dependencies

# Build and run the project
echo "Building and running the project..."
su - "$user" -c "
  #export DISPLAY=:0
  #export XDG_RUNTIME_DIR=/run/user/\$(id -u)
  #export XAUTHORITY=/home/$user/.Xauthority
  cd '$DIR'
  cmake -B build -S .
  cmake --build build
  #./build/app/app
"
