
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libgui)

# List file here
add_library(${lib}
    gui.cpp        gui.hpp
    widget.cpp     widget.hpp
    gridlayout.cpp gridlayout.hpp
    frame.cpp      frame.hpp
    item.cpp       item.hpp
    list.cpp       list.hpp
    scrollbar.cpp  scrollbar.hpp
    fillbar.cpp    fillbar.hpp
    checkbox.cpp   checkbox.hpp
    radio.cpp      radio.hpp
    text.cpp       text.hpp
	)

