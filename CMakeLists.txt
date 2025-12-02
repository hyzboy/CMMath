cmake_minimum_required(VERSION 3.28)

project(CMMath)

include(path_config.cmake)
CMMathSetup(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMMATH_ROOT_SOURCE_PATH})
