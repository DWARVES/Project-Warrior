
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libcore)

# List file here
add_library(${lib}
	fakefs-def.hpp fakefs.hpp
    pathParser.cpp pathParser.hpp
	)

