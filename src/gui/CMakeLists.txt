
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
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
    input.cpp      input.hpp
    button.cpp     button.hpp
    theme.cpp      theme.hpp
    image.cpp      image.hpp
	)

