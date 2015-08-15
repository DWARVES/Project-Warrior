
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib libgraphics)

# List file here
add_library(${lib}
    graphics.cpp graphics.hpp
    texture.cpp  texture.hpp
    movie.cpp    movie.hpp
    font.cpp     font.hpp
    color.cpp    color.hpp
    exts.cpp     exts.hpp
    shaders.cpp  shaders.hpp
	)

