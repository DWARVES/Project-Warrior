
include_directories(${DEMO_SOURCE_DIR}/src)
include_directories(${DEMO_SOURCE_DIR}/tests/physics)
link_directories(${DEMO_BINARY_DIR}/src)
link_directories(${DEMO_BINARY_DIR}/tests/physics)

# Each test here
add_subdirectory(Entity-test)


