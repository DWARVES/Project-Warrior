
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib menus)

# List file here
add_library(${lib}
    menu.cpp             menu.hpp
    mainmenu.cpp         mainmenu.hpp
    gamemenu.cpp         gamemenu.hpp
    endmenu.cpp          endmenu.hpp
	)

