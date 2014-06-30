if(WIN32)
    MESSAGE("WIN32: Compiling with mingw32 and linking SFML statically.")
    SET( CMAKE_MAKE_PROGRAM "C:/Program Files (x86)/CodeBlocks/MinGW/bin/mingw32-make")
    SET(CMAKE_CODEBLOCKS_EXECUTABLE "C:/Program Files (x86)/CodeBlocks/codeblocks")
elseif(UNIX)
    MESSAGE("UNIX: Compiling with gcc and linking SFML dinamically")
endif()

#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)
project(GO_INFLUENCE_MAP CXX)

# Set version information in a config.h file
set(go_influence_map_VERSION_MAJOR 1)
set(go_influence_map_VERSION_MINOR 0)

#Setting GoGUI files
INCLUDE(GoGUI/CMakeLists.txt)
FOREACH(FILE ${FILES})
        SET (GoGUIFiles ${GoGUIFiles} GoGUI/${FILE})
ENDFOREACH (FILE)

SET(HEADERS Config.h )
SET(SOURCES main.cpp GoInfluenceMap.cpp)

#If your processor is 64bits, set -m64, otherwise -m32.
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=gnu++0x")
elseif(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -std=c++0x -mwindows ")
endif()

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

#The go_utils headers
include_directories("${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_UTILS/")

#Go Database library
include_directories("${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_MEMORY/")

#Go MCTS library
include_directories("${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_MCTS/")

if(UNIX)
    #This sets where you have installed SFML
    set(SFML_ROOT "/home/gabriel/libraries/SFML-2.1")

    #Go Utils lib
    set(Go_UTILS_library "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_UTILS/libgo_utils.so")

    #Go Database lib
    set(GO_DATABASE "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_MEMORY/libgo_memory.so")

    #Go MCTS Lib
    set(GO_MCTS "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_MCTS/libgo_mcts.so")


#REVIEW WIN CONFIG
elseif(WIN32)
    set(Go_UTILS_library "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_UTILS/libgo_utils.dll")

    #Go Database lib
    set(GO_DATABASE "${ROOT_PROJECT_BINARY_DIR}/LIB_GO_MEMORY/libgo_memory.dll")

    set(SFML_ROOT "C:/Program Files (x86)/SFML/")
    set(SFML_EXT  "C:/Users/gabriel.santos/Downloads/SFML-master/SFML-master")
    set(BOOST_ROOT "C:/Users/gabriel.santos/Downloads/boost_1_55_0/boost_1_55_0")
    set(MINGW_ROOT "C:/Program Files (x86)/CodeBlocks/MinGW")

    include_directories(${SFML_EXT}/extlibs/headers)
    include_directories(${SFML_ROOT}/include)
    include_directories(${BOOST_ROOT})

    ADD_DEFINITIONS( -DSFML_STATIC -DGLEW_STATIC  )
endif()

MESSAGE("LIB GO: " ${Go_UTILS_library})
MESSAGE("LIB GO DATABASE: " ${GO_DATABASE})
MESSAGE("LIB GO_MCTS: " ${GO_MCTS})
MESSAGE("INCLUDES: ${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_UTILS/")
MESSAGE("INCLUDES: ${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_MEMORY/" )
MESSAGE("INCLUDES: ${ROOT_PROJECT_SOURCE_DIR}/LIB_GO_MCTS/" )

# Define sources and executable
set(EXECUTABLE_NAME "go_influence_map")
add_executable(${EXECUTABLE_NAME} ${HEADERS} ${SOURCES} ${GoGUIFiles} ${InfluenceModelFiles} ${InfluenceStatisticsFiles})

if(UNIX)
    #Create a CMakeFiles/cmake_modules tree directory then copy the SFML.cmake file (Found on the SFML2.1 folder), or just point this to the SFML.cmake file.
    set(CMAKE_SOURCE_DIR "/home/gabriel/CMakeFiles")

    #Install boost (Currently using boost_1.55.0)
    #Install libglew-dev (It must be libglew1.7 or later)

    # Detect and add SFML
    set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
    #Find any version 2.X of SFML
    #See the FindSFML.cmake file for additional details and instructions
    find_package(SFML 2 REQUIRED system window graphics network audio)
    if(SFML_FOUND)
      include_directories(${SFML_INCLUDE_DIR} )
      target_link_libraries(${EXECUTABLE_NAME}  ${Go_UTILS_library}  ${GO_DATABASE} ${GO_MCTS} ${SFML_LIBRARIES} GLEW)
    endif()

elseif(WIN32)

    #Add SFML libraries and its dependencies statically
    add_library(sfml_main STATIC IMPORTED)
    add_library(sfml_graphics STATIC IMPORTED)
    add_library(sfml_window STATIC IMPORTED)
    add_library(sfml_system STATIC IMPORTED)
    add_library(freetype STATIC IMPORTED)
    add_library(glew STATIC IMPORTED)
    add_library(jpeg STATIC IMPORTED)
    add_library(openal32 STATIC IMPORTED)
    add_library(sndfile STATIC IMPORTED)
    add_library(opengl32 STATIC IMPORTED)
    add_library(winmm STATIC IMPORTED)
    add_library(gdi32 STATIC IMPORTED)

    #Setting locations
    set_target_properties(sfml_main PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-main.a)

    set_target_properties(sfml_graphics PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-graphics-s.a)

    set_target_properties(sfml_window PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-window-s.a)

    set_target_properties(sfml_system PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-system-s.a)

    set_target_properties(freetype PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libfreetype.a)

    set_target_properties(glew PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libglew.a)

    set_target_properties(jpeg PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libjpeg.a)

    set_target_properties(openal32 PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libopenal32.a)

    set_target_properties(sndfile PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsndfile32.a)

    set_target_properties(opengl32 PROPERTIES
           IMPORTED_LOCATION ${MINGW_ROOT}/lib/libopengl32.a)

    set_target_properties(winmm PROPERTIES
           IMPORTED_LOCATION ${MINGW_ROOT}/lib/libwinmm.a)

    set_target_properties(gdi32 PROPERTIES
           IMPORTED_LOCATION ${MINGW_ROOT}/lib/libgdi32.a)

    #Add SFML debug libraries
    add_library(sfml_main_d STATIC IMPORTED)
    add_library(sfml_graphics_d STATIC IMPORTED)
    add_library(sfml_window_d STATIC IMPORTED)
    add_library(sfml_system_d STATIC IMPORTED)

    #Setting locations
    set_target_properties(sfml_main_d PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-main-d.a)

    set_target_properties(sfml_graphics_d PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-graphics-s-d.a)

    set_target_properties(sfml_window_d PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-window-s-d.a)

    set_target_properties(sfml_system_d PROPERTIES
           IMPORTED_LOCATION ${SFML_ROOT}/lib/libsfml-system-s-d.a)

    #linking config
    SET(link_sfml_main optimized sfml_main debug sfml_main_d)
    SET(link_sfml_graphics optimized sfml_graphics debug sfml_graphics_d)
    SET(link_sfml_window optimized sfml_window debug sfml_window_d)
    SET(link_sfml_system optimized sfml_system debug sfml_system_d)
    SET(link_freetype optimized freetype)
    SET(link_opengl32 optimized opengl32)
    SET(link_glew optimized glew)
    SET(link_jpeg optimized jpeg)
    SET(link_openal32 optimized openal32)
    SET(link_winmm optimized winmm)
    SET(link_gdi32 optimized gdi32)
    SET(link_sndfile32 optimized sndfile)

    #Linking libraries with application
    TARGET_LINK_LIBRARIES( ${EXECUTABLE_NAME}
            ${GO_DATABASE}
            ${Go_UTILS_library}
            ${link_sfml_main}
            ${link_sfml_graphics}
            ${link_sfml_window}
            ${link_sfml_system}
            ${link_freetype}
            ${link_glew}
            ${link_jpeg}
            ${link_openal32}
            ${link_sndfile}
            ${link_opengl32}
            ${link_winmm}
            ${link_gdi32}            
    )

    #Setting properties
    SET_TARGET_PROPERTIES(${EXECUTABLE_NAME}
        PROPERTIES
        COMPILE_DEFINITIONS
        "GLEW_STATIC="
)

    # Install target
    #install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
endif()
