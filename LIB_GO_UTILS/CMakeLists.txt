if(WIN32)
    MESSAGE("WIN32: Compiling LibGoUtils with mingw32.")
    SET( CMAKE_MAKE_PROGRAM "C:/Program Files (x86)/CodeBlocks/MinGW/bin/mingw32-make")
    SET(CMAKE_CODEBLOCKS_EXECUTABLE "C:/Program Files (x86)/CodeBlocks/codeblocks")
elseif(UNIX)
    MESSAGE("UNIX: Compiling LibGoUtils with GCC")
endif()


#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(LIB_GO_UTILS CXX)

# Set version information in a config.h file
set(lib_go_utils_VERSION_MAJOR 1)
set(lib_go_utils_VERSION_MINOR 0)

#Setting Influence model files
INCLUDE(GoInfluenceModel/CMakeLists.txt)
FOREACH(FILE ${FILES})
        SET (InfluenceModelFiles ${InfluenceModelFiles} GoInfluenceModel/${FILE})
ENDFOREACH (FILE)

#Setting Influence Statistics files
INCLUDE(GoInfluenceStatistics/CMakeLists.txt)
FOREACH(FILE ${FILES})
        SET (InfluenceStatisticsFiles ${InfluenceStatisticsFiles} GoInfluenceStatistics/${FILE})
ENDFOREACH (FILE)

SET(HEADERS I_GobanObserver.h I_GobanSubject.h GoUtils.h GobanUtils.h)
SET(SOURCES Goban.cpp GoReferee.cpp  GoGame.cpp  SimpleGoGame.cpp GoGameTree.cpp)

#If your processor is 64bits, set -m64, otherwise -m32.
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=gnu++0x")
elseif(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=c++0x")
endif()

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories(${PROJECT_BINARY_DIR})

if(WIN32)
    set(BOOST_ROOT "C:/Users/gabriel.santos/Downloads/boost_1_55_0/boost_1_55_0")
    set(MINGW_ROOT "C:/Program Files (x86)/CodeBlocks/MinGW")
    include_directories(${BOOST_ROOT})
endif()

# Define sources and executable
set(LIBRARY_NAME "go_utils")
#Creating SHARED library
add_library(${LIBRARY_NAME} SHARED ${HEADERS} ${SOURCES} ${InfluenceModelFiles} ${InfluenceStatisticsFiles})

