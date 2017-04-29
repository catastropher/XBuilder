# XBuilder
The level editor for the X3D game engine!

## Dependencies
You will need:
- CMake
- X3D (https://github.com/catastropher/X3D-68k)
- OpenGL 3 (check /usr/include/ for GLES3)
- SDL 2 (libsdl2-dev)
- Boost Filesystem (libboost-filesystem-dev)

## Build Instructions
```
git clone git@github.com:catastropher/XBuilder.git
cd XBuilder
mkdir build
cd build
cmake ..
make -j 4
```
