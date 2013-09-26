

include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(exe warrior)

# List file here
add_executable(${exe}
	main.cpp)
target_link_libraries(
    ${exe}
    ${Boost_REGEX_LIBRARY}
    ${LUA_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDLMIXER_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${FFMPEG_LIBRARIES}
    ${GLEW_LIBRARIES}
    )

add_subdirectory(core)
target_link_libraries(${exe} libcore)
add_subdirectory(events)
target_link_libraries(${exe} libevents)
add_subdirectory(geometry)
target_link_libraries(${exe} libgeometry)
add_subdirectory(lua)
target_link_libraries(${exe} liblua)
add_subdirectory(physics)
target_link_libraries(${exe} libphysics)
add_subdirectory(graphics)
target_link_libraries(${exe} libgraphics)

