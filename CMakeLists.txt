cmake_minimum_required(VERSION 2.6)

project(Project-Warrior)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules/")

# Including Boost
include(FindBoost)
find_package(Boost COMPONENTS regex REQUIRED)
link_directories(${Boost_REGEX_LIBRARY_DIR})
include_directories(${Boost_REGEX_INCLUDE_DIR})

# Including de la SDL2
include(FindSDL2)
find_package(SDL2 REQUIRED)
link_directories(${SDL2_LIBRARY_DIR})
include_directories(${SDL2_INCLUDE_DIR})

# Inclufing de SDL2_image
include(FindSDL2_image)
find_package(SDL2_image REQUIRED)
link_directories(${SDL2_IMAGE_LIBRARY_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})

# Including OpenGL
include(FindOpenGL)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Including Lua
include(FindLua)
find_package(Lua 5.2 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Includes checks
include(CheckIncludeFiles)
check_include_files("sys/types.h;sys/stat.h" HAVE_SYSSTAT_H)
check_include_files("dirent.h" HAVE_DIRENT_H)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(src/)
add_subdirectory(src/)

if(TESTS)
	add_subdirectory(tests/)
else()
	add_subdirectory(tests/ EXCLUDE_FROM_ALL)
endif(TESTS)

