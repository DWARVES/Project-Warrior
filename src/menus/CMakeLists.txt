
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib menus)

# List file here
add_library(${lib}
    menu.cpp     menu.hpp
	)

