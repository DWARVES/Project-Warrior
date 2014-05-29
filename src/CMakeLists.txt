
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(exe warrior)

# List file here
add_executable(${exe}
	main.cpp
    global.cpp global.hpp
    )

add_subdirectory(menus)
set(libs "menus")
add_subdirectory(gameplay)
set(libs "${libs};libgameplay")
add_subdirectory(lua)
set(libs "${libs};liblua")
add_subdirectory(Box2D)
set(libs "${libs};Box2D")
add_subdirectory(physics)
set(libs "${libs};libphysics")
add_subdirectory(graphics)
set(libs "${libs};libgraphics")
add_subdirectory(gui)
set(libs "${libs};libgui")
add_subdirectory(events)
set(libs "${libs};libevents")
add_subdirectory(liblua)
set(libs "${libs};liblua5.2")
add_subdirectory(geometry)
set(libs "${libs};libgeometry")
add_subdirectory(core)
set(libs "${libs};libcore")

target_link_libraries(
    ${exe}
    ${libs}
    ${libs}
    ${Boost_REGEX_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDLMIXER_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    )

