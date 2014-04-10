#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(GO_TOOLS CXX)

MESSAGE("SOURCE DIR: "${PROJECT_SOURCE_DIR})
MESSAGE("BINARY DIR: "${PROJECT_BINARY_DIR})

SET(ROOT_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
SET(ROOT_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})

add_subdirectory(LIB_GO_UTILS)
add_subdirectory(GO_GUI)


