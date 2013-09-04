
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests)

# Each subdirectory has its own tests
add_subdirectory(core)
add_subdirectory(events)
add_subdirectory(geometry)
add_subdirectory(lua)
add_subdirectory(physics)
