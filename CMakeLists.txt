cmake_minimum_required(VERSION 2.6)

project(XBuilder)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g -fsanitize=address -fsanitize=undefined -Wno-pointer-sign ")

add_executable(XBuilder
  src/main.c
  src/texturepack.c
  src/linetexture_editor.c
  src/command.c
  src/mouse.c
)

set(Agar_DIR ".")

find_package(SDL REQUIRED)
find_package(Agar REQUIRED)
target_link_libraries(XBuilder $ENV{X3D}/build/X3D/libX3D.a)
target_link_libraries(XBuilder SDLmain ${SDL_LIBRARY})
target_link_libraries(XBuilder m)

include_directories($ENV{X3D}/src/headers)
include_directories(${LibAgar_INCLUDE_DIRS})


# Link the target to the GTK+ libraries
target_link_libraries(XBuilder ${GTK2_LIBRARIES})
target_link_libraries(XBuilder ${LibAgar_LIBRARIES})
