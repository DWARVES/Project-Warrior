
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
set(lib libcore)

# List file here
add_library(${lib}
	fakefs-def.hpp fakefs.hpp
    pathParser.cpp pathParser.hpp
    systemtime.cpp systemtime.hpp
    logger.cpp     logger.hpp
    config.cpp     config.hpp
    utf8.cpp       utf8.hpp
    i18n.cpp       i18n.hpp
	)

