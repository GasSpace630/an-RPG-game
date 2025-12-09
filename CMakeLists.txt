# Use this to build
# cmake --build . -j

# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# The name of your project (This will be the name of your executable)
project(game CXX)

# Specify the standard C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. FIND RAYLIB
# This command searches for the installed raylib package configuration.
# If raylib is installed correctly, this finds the necessary include paths and library files.
find_package(raylib CONFIG REQUIRED)

# 2. DEFINE SOURCE FILES
set(SOURCE_FILES
    src/enemy.cpp
    src/player.cpp
    main.cpp 
    # Add any other .cpp files here
)

# 3. ADD EXECUTABLE TARGET
add_executable(
    ${PROJECT_NAME} 
    ${SOURCE_FILES}
)

# 4. ADD PROJECT INCLUDE DIRECTORY
# This allows you to use your custom headers like #include "entity.h"
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)