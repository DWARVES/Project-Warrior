
include_directories(${DEMO_SOURCE_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/src)
set(lib liblua)

# List file here
add_library(${lib}
    saveExposure.cpp     saveExposure.hpp
    graphicsExposure.cpp graphicsExposure.hpp
    pathExposure.cpp     pathExposure.hpp
    charaExposure.cpp    charaExposure.hpp
    stageExposure.cpp    stageExposure.hpp
    script.cpp           script.hpp           luna.hpp
    helper.cpp           helper.hpp
    exception.cpp        exception.hpp
	)

