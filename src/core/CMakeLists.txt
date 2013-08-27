
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib libcore)

# List file here
add_library(${lib}
	fakefs.cpp fakefs.hpp
	)

