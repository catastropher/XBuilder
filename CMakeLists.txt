cmake_minimum_required(VERSION 2.6)

project(XBuilder)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g -fsanitize=address -fsanitize=undefined -Wno-pointer-sign ")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fsanitize=address -fsanitize=undefined")

add_executable(XBuilder
  src/main.cpp
  src/level/Level.cpp
  src/keys.cpp
  src/texture.cpp
  
  src/gui/tools/ToolManager.cpp
  
  src/pack/DirectoryScanner.cpp
  src/pack/ResourcePack.cpp
  
  src/imgui/imgui_impl_sdl_gl3.cpp
  src/imgui/imgui.cpp
  src/imgui/imgui_draw.cpp
  
  src/gui/gui.cpp
)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(OpenGL REQUIRED VERSION 3)

target_link_libraries(XBuilder ${Boost_LIBRARIES})
target_link_libraries(XBuilder /usr/local/lib/libX3D.a)
target_link_libraries(XBuilder SDLmain SDL2)
target_link_libraries(XBuilder m)
target_link_libraries(XBuilder ${OPENGL_LIBRARIES})

include_directories(/usr/local/include/X3D)
include_directories(src/)

