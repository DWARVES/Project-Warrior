
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libgraphics)

# List file here
add_library(${lib}
    graphics.cpp graphics.hpp
    texture.cpp  texture.hpp
    font.cpp     font.hpp
    color.cpp    color.hpp
    exts.cpp     exts.hpp
    shaders.cpp  shaders.hpp
	)

