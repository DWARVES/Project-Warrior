
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libgameplay)

# List file here
add_library(${lib}
    save.cpp      save.hpp
    character.cpp character.hpp
    controler.cpp controler.hpp
    stage.cpp     stage.hpp
	)

