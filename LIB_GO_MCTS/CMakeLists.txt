#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(LIB_GO_MCTS CXX)

# Set version information in a config.h file
set(lib_go_mcts_VERSION_MAJOR 1)
set(lib_go_mcts_VERSION_MINOR 0)

#Configure file
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

SET(HEADERS I_PlayOutPolicy.h I_TreePolicy.h MCTree.h)
SET(SOURCES MctsPlayer.cpp UCTPolicy.cpp PlayOutSearcher.cpp RandomPolicy.cpp)

#If your processor is 64bits, set -m64, otherwise -m32.
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=gnu++0x -lboost_filesystem -lboost_system")
elseif(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=c++0x -lboost_filesystem -lboost_system")
endif()

#include_directories("${PROJECT_SOURCE_DIR}/LIB_GO_UTILS/")
#include_directories("/home/gabriel/coding/GO_TOOLS/LIB_GO_UTILS/")
#set(ROOT_PROJECT_SOURCE_DIR "/home/gabriel/coding/GO_TOOLS/")
#set(ROOT_PROJECT_BINARY_DIR "/home/gabriel/coding/GO_TOOLS-build/")
include_directories("${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_UTILS/")
include_directories(${PROJECT_BINARY_DIR})

if(UNIX)
    #Go Utils lib
    set(Go_UTILS_library "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_UTILS/libgo_utils.so")

elseif(WIN32)
    set(Go_UTILS_library "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_UTILS/libgo_utils.dll")
    set(BOOST_ROOT "C:/Users/gabriel.santos/Downloads/boost_1_55_0/boost_1_55_0")
    set(MINGW_ROOT "C:/Program Files (x86)/CodeBlocks/MinGW")
endif()

if(WIN32)
    set(BOOST_ROOT "C:/Users/gabriel.santos/Downloads/boost_1_55_0/boost_1_55_0")
    set(MINGW_ROOT "C:/Program Files (x86)/CodeBlocks/MinGW")
    include_directories(${BOOST_ROOT})
endif()

# Define sources and executable
set(LIBRARY_NAME "go_mcts")
#Creating SHARED library
add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES})

# Define sources and executable
#set(EXECUTABLE_NAME "go_memory_map")
#add_executable(${EXECUTABLE_NAME} ${HEADERS} ${SOURCES})

if(UNIX)

target_link_libraries(${LIBRARY_NAME} ${Go_UTILS_library})
#target_link_libraries(${EXECUTABLE_NAME} ${Go_UTILS_library})

endif()
