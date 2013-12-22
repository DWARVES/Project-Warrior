
set(entries
    font.t2t
    fakefs.t2t
    )

foreach(entry ${entries})
    string(REGEX REPLACE ".t2t" "" target ${entry})
    string(REGEX REPLACE ".t2t" ".html" dest ${entry})
    set(target "wiki-${target}")
    list(APPEND targets ${target})
    set(entry "${CMAKE_CURRENT_SOURCE_DIR}/${entry}")
    add_custom_target(${target} txt2tags -t html ${T2TFLAGS} -o ${dest} ${entry}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating the wiki page for ${entry}." VERBATIM)
endforeach()

add_custom_target(wiki DEPENDS ${targets}
    COMMENT "Generated all wiki pages." VERBATIM
    )

