
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/gui)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/gui)

# Each test here
add_executable(gui-test gui-test.cpp)
target_link_libraries(gui-test libgui libevents libgraphics libcore libgeometry
    ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_REGEX_LIBRARY})
add_executable(gridlayout-test gridlayout-test.cpp)
target_link_libraries(gridlayout-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(frame-test frame-test.cpp)
target_link_libraries(frame-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(list-test list-test.cpp)
target_link_libraries(list-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(misc1-test misc1-test.cpp)
target_link_libraries(misc1-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(scrollbar-test scrollbar-test.cpp)
target_link_libraries(scrollbar-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(fillbar-test fillbar-test.cpp)
target_link_libraries(fillbar-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(checkbox-test checkbox-test.cpp)
target_link_libraries(checkbox-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(radio-test radio-test.cpp)
target_link_libraries(radio-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(text-test text-test.cpp)
target_link_libraries(text-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(input-test input-test.cpp)
target_link_libraries(input-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(button-test button-test.cpp)
target_link_libraries(button-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})
add_executable(misc2-test misc2-test.cpp)
target_link_libraries(misc2-test libgui libevents libgraphics libcore libgeometry
    ${Boost_REGEX_LIBRARY} ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES})

