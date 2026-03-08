
add_library(quartz_lib
)

set(LIB_HEADER_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/header.stamp")

target_include_directories(quartz_lib PUBLIC include/)

set(QUARTZ_LIB_HEADER_FILES
)

set(QUARTZ_LIB_HEADER_DIR "/include/quartz/lib/")

add_custom_command(
    OUTPUT "${LIB_HEADER_STAMP}"
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QUARTZ_LIB_HEADER_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_LIB_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${LIB_HEADER_STAMP}"
    DEPENDS ${HEADER_FILES}
    COMMENT "Copying lib headers"
    VERBATIM
)

add_custom_target(quartz_lib_headers
    DEPENDS "${LIB_HEADER_STAMP}"
)

add_dependencies(quartz_lib quartz_lib_headers)
