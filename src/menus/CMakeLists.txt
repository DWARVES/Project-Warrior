
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib menus)

# List file here
add_library(${lib}
    buttonmenu.cpp       buttonmenu.hpp
    menu.cpp             menu.hpp
    mainmenu.cpp         mainmenu.hpp
    aboutmenu.cpp        aboutmenu.hpp
    configmenu.cpp       configmenu.hpp
    graphicsmenu.cpp     graphicsmenu.hpp
    musicmenu.cpp        musicmenu.hpp
    videomenu.cpp        videomenu.hpp
    controlermenu.cpp    controlermenu.hpp
    addcontrolermenu.cpp addcontrolermenu.hpp
    playerselmenu.cpp    playerselmenu.hpp
    charaselmenu.cpp     charaselmenu.hpp
    stageselmenu.cpp     stageselmenu.hpp
    gamemenu.cpp         gamemenu.hpp
    endmenu.cpp          endmenu.hpp
	)

