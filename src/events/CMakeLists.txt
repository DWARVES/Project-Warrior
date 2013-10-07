
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libevents)

# List file here
add_library(${lib}
    events.cpp  events.hpp
    key.cpp     key.hpp
    keytype.hpp keymap.hpp
    button.hpp  windowstate.hpp
	)

