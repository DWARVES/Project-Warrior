
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libgui)

# List file here
add_library(${lib}
    gui.cpp        gui.hpp
    widget.cpp     widget.hpp
    gridlayout.cpp gridlayout.hpp
    frame.cpp      frame.hpp
	)

