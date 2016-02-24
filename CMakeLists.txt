cmake_minimum_required(VERSION 2.6)

project(XBuilder)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK2_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK2_CFLAGS_OTHER})

set(CMAKE_C_FLAGS "-std=c99 -Wall -g -fsanitize=address -fsanitize=undefined -Wno-pointer-sign")

add_executable(XBuilder
  src/main.c
)

find_package(SDL REQUIRED)
target_link_libraries(XBuilder $ENV{X3D}/build/X3D/libX3D.a)
target_link_libraries(XBuilder SDLmain ${SDL_LIBRARY})
target_link_libraries(XBuilder m)

include_directories($ENV{X3D}/src/headers)

# Link the target to the GTK+ libraries
target_link_libraries(XBuilder ${GTK2_LIBRARIES})