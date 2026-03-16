
add_library(quartz_lib
src/monolayer_timeline_system.cpp
src/monolayer_timeline_system.hpp
src/library_system.hpp
src/library_system.cpp
)

target_include_directories(quartz_lib PUBLIC include/)

set(QUARTZ_LIB_HEADER_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/src/monolayer_timeline_system.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/library_system.hpp"
)

set(QUARTZ_LIB_HEADER_DIR "/include/quartz/lib/")

add_custom_target(quartz_lib_headers ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_LIB_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_HEADER_DIR}"
    DEPENDS ${QUARTZ_LIB_HEADER_FILES}
    COMMENT "Copying lib headers"
    VERBATIM
)

add_dependencies(quartz_lib quartz_lib_headers)
