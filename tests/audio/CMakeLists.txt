
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/audio)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/audio)

# Each test here
add_executable(audio-test audio-test.cpp)
target_link_libraries(audio-test libaudio libcore libgraphics ${SDLMIXER_LIBRARY} libgeometry ${OPENGL_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${FFMPEG_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_REGEX_LIBRARY})

