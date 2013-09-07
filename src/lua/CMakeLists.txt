
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib liblua)

# List file here
add_library(${lib}
    script.cpp    script.hpp    luna.hpp
    helper.cpp    helper.hpp
    exception.cpp exception.hpp
	)

