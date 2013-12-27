
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib menus)

# List file here
add_library(${lib}
    buttonmenu.cpp   buttonmenu.hpp
    menu.cpp         menu.hpp
    mainmenu.cpp     mainmenu.hpp
    aboutmenu.cpp    aboutmenu.hpp
    configmenu.cpp   configmenu.hpp
    graphicsmenu.cpp graphicsmenu.hpp
	)

