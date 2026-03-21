
add_library(quartz_lib_parser
src/prophase.cpp
src/prophase.hpp
src/metaphase.cpp
src/metaphase.hpp
src/anaphase.hpp
src/anaphase.cpp
)

target_include_directories(quartz_lib_parser PUBLIC include)

set(QUARTZ_LIB_PARSER_HEADER_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/src/prophase.hpp"
)

set(QUARTZ_LIB_PARSER_HEADER_DIR "/include/quartz/lib/parser/")

add_custom_target(quartz_lib_parser_headers ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_PARSER_HEADER_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_LIB_PARSER_HEADER_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_PARSER_HEADER_DIR}
    DEPENDS ${QUARTZ_LIB_PARSER_HEADER_FILES}
    COMMENT "Copying core_lib headers"
    VERBATIM
)

add_dependencies(quartz_lib_parser quartz_lib_parser_headers)
