
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests/core)
link_directories(${CMAKE_BINARY_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/tests/core)

# Each test here
add_executable(fakefs-test fakefs-test.cpp)
target_link_libraries(fakefs-test libcore ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
add_executable(pathParser-test pathParser-test.cpp)
target_link_libraries(pathParser-test libcore ${Boost_FILESYSTEM_LIBRARY})
add_executable(systemTime-test systemTime-test.cpp)
target_link_libraries(systemTime-test libcore)
add_executable(logger-test logger-test.cpp)
target_link_libraries(logger-test libcore)
add_executable(config-test config-test.cpp)
target_link_libraries(config-test libcore ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
add_executable(utf8-test utf8-test.cpp)
target_link_libraries(utf8-test libcore)


