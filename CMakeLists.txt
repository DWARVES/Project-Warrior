cmake_minimum_required(VERSION 2.6)

project(Project-Warrior)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

# TODO Add dependencies like SDL, OpengL, Box2D ...

include_directories(src/)
add_subdirectory(src/)

if(TESTS)
	add_subdirectory(tests/)
else()
	add_subdirectory(tests/ EXCLUDE_FROM_ALL)
endif(TESTS)

