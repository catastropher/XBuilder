cmake_minimum_required(VERSION 2.6)

project(XBuilder)

set(CMAKE_C_FLAGS "-std=c99 -Wall -g -fsanitize=address -fsanitize=undefined -Wno-pointer-sign ")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fsanitize=address -fsanitize=undefined -Wno-pointer-sign")

add_executable(XBuilder
  src/main.cpp
  src/levelbuilder.cpp
  src/keys.cpp
  src/tool.cpp
  
  src/pack/DirectoryScanner.cpp
  src/pack/ResourcePack.cpp
)

find_package(Boost REQUIRED COMPONENTS filesystem)

target_link_libraries(XBuilder ${Boost_LIBRARIES})
target_link_libraries(XBuilder /usr/local/lib/libX3D.a)
target_link_libraries(XBuilder SDLmain SDL2)
target_link_libraries(XBuilder m)

include_directories(/usr/local/include/X3D)

