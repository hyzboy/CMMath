cmake_minimum_required(VERSION 3.28)

project(CMMath LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(path_config.cmake)
CMMathSetup(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMMATH_ROOT_SOURCE_PATH})

add_subdirectory(examples)
