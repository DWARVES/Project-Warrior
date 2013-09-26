
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libaudio)

# List file here
add_library(${lib}
    audio.cpp audio.hpp
    music.cpp music.hpp
    sound.cpp sound.hpp
	)

