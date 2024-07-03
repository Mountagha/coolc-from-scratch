# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# set the c++ version
set(CMAKE_CXX_STANDARD 17)
# Set the project name
project (cool-compiler)

add_compile_options(-g)

include_directories("includes")
file(GLOB SOURCES "src/*.cpp")

# Add an executable
add_executable(coolc ${SOURCES})