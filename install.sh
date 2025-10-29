#!/bin/bash
set -e

user=$(logname)
DIR=$(pwd)

if [ "$EUID" -ne 0 ]; then
  echo "This script requires root permissions."
  echo "It will be executed again with su -c"
  exec su -c "$0"
fi


install_gplusplus(){
  if command -v g++ &>/dev/null; then
    echo "G++ is already installed. Version: $(g++ --version | head -n1)"
  else
    if command -v apt &>/dev/null; then
      apt update
      apt install -y g++
    fi
  fi
}

install_cmake(){
  if command -v cmake &>/dev/null; then
    echo "CMake is already installed. Version: $(cmake --version | head -n1)"
  else
    if command -v apt &>/dev/null; then
      apt update
      apt install -y cmake
    fi
  fi
}


echo "Installign G++..."
install_gplusplus
echo "Installign CMake..."
install_cmake
echo "Installing SDL dependencies"
apt-get install -y libsdl2-dev libglm-dev libglfw3 libglew-dev libx11-dev

su - "$user" -c "
    cd '$DIR'
    cmake -B build -S .
    cmake --build build
    ./build/app/app
"
#cd "$DIR"
#cmake -B build -S .
#cmake --build build
#./build/app/app

# This program is compiled without root permissions
#su "$user" -c "cd '$DIR' && make"
#cp "$DIR/build/huff" /usr/bin
#cp "$DIR/build/dehuff" /usr/bin
