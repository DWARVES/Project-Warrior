cmake_minimum_required(VERSION 2.6)

project(Project-Warrior)
set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules/")

# Including Boost
include(FindBoost)
find_package(Boost COMPONENTS regex REQUIRED)
link_directories(${Boost_REGEX_LIBRARY_DIR})
include_directories(SYSTEM ${Boost_REGEX_INCLUDE_DIR})

# Including SDL2
include(FindSDL2)
find_package(SDL2 REQUIRED)
link_directories(${SDL2_LIBRARY_DIR})
include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

# Including SDL2_image
include(FindSDL2_image)
find_package(SDL2_image REQUIRED)
link_directories(${SDL2_IMAGE_LIBRARY_DIR})
include_directories(SYSTEM ${SDL2_IMAGE_INCLUDE_DIR})

# Including OpenGL
include(FindOpenGL)
find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})

# Including GLEW
include(FindGLEW)
find_package(GLEW REQUIRED)
include_directories(SYSTEM ${GLEW_INCLUDE_DIR})


# Includes checks
include(CheckIncludeFiles)
check_include_files("libintl.h;locale.h" HAVE_LIBINTL_H)
check_include_files("sys/types.h;sys/stat.h" HAVE_SYSSTAT_H)
check_include_files("dirent.h" HAVE_DIRENT_H)

find_package(Boost COMPONENTS filesystem)
if(Boost_FILESYSTEM_FOUND)
    set(HAVE_BOOST_FILESYSTEM 1)
else()
    set(HAVE_BOOST_FILESYSTEM 0)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Documentation generation
include(FindDoxygen)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating source code documentation with Doxygen (for devs only)." VERBATIM
        )
elseif(DOXYGEN_FOUND)
    message("Couldn't find Doxygen : won't be able to generate doc.")
endif(DOXYGEN_FOUND)

# i18n support with gnu gettext
set(DOMAIN "warrior")
# Files with text to translate.
set(SRCS src/main.cpp
    src/menus/mainmenu.cpp
    src/menus/aboutmenu.cpp
    )
add_custom_target(extract_text xgettext -d ${DOMAIN} --c++ -i -F -k_i -o ${DOMAIN}.pot ${SRCS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Extract the translatable texts from the source code with xgettext." VERBATIM
    )

include_directories(src/)
add_subdirectory(src/)

if(TESTS)
	add_subdirectory(tests/)
else()
	add_subdirectory(tests/ EXCLUDE_FROM_ALL)
endif(TESTS)

if(TOOLS)
    add_subdirectory(tools/)
else()
    add_subdirectory(tools/ EXCLUDE_FROM_ALL)
endif(TOOLS)

add_subdirectory(wiki/)


