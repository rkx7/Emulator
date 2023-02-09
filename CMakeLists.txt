project(Emulator)
cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE EmulatorSources "Source/*.hxx" "Source/*.cxx")

add_executable(emulator ${EmulatorSources})