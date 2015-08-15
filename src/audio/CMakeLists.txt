
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib libaudio)

# List file here
add_library(${lib}
    audio.cpp audio.hpp
    music.cpp music.hpp
    sound.cpp sound.hpp
	)

