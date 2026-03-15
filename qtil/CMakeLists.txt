
add_library(qtil
src/overloads.hpp
src/panic.hpp
src/panic.cpp
src/vector_util.hpp
)

set(QTIL_HEADER_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/src/overloads.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/panic.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/vector_util.hpp"
)

set(QTIL_HEADER_DIR "/include/qtil/")

add_custom_target(qtil_headers ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}${QTIL_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QTIL_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}${QTIL_HEADER_DIR}"
    DEPENDS ${QTIL_HEADER_FILES}
    COMMENT "Copying core headers"
    VERBATIM
)

target_include_directories(qtil PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_dependencies(qtil qtil_headers)
