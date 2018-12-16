#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(GO_TOOLS CXX)

MESSAGE("SOURCE DIR: " ${PROJECT_SOURCE_DIR} )
MESSAGE("BINARY DIR: " ${PROJECT_BINARY_DIR} )

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/opt/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/opt/boost/stage/lib")

find_package(Boost 1.55.0 REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    MESSAGE("Boost FOUND!")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${CMAKE_LIBRARY_PATH})
else()
    MESSAGE("ERROR ### Boost not found ### ")
endif()

SET(ROOT_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
SET(ROOT_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})

add_subdirectory(LIB_GO_UTILS)
add_subdirectory(LIB_GO_MEMORY)
add_subdirectory(LIB_GO_MCTS)
add_subdirectory(GO_GUI)


