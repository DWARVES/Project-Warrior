
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/liblua/lua-5.2.3/src)
include_directories(${CMAKE_SOURCE_DIR}/tests)
link_directories(${CMAKE_BINARY_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/tests)

# Each subdirectory has its own tests
add_subdirectory(core)
add_subdirectory(gameplay)
add_subdirectory(geometry)
add_subdirectory(lua)
add_subdirectory(physics)
add_subdirectory(graphics)
add_subdirectory(audio)
add_subdirectory(events)
add_subdirectory(gui)

