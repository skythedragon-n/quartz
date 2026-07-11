
add_library(quartz_renderer_coesite
src/Scene.cpp
src/Scene.hpp
)

target_include_directories(quartz_renderer_coesite PUBLIC include)

set(QUARTZ_RENDERER_COESITE_HEADER_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/src/Scene.hpp"
)

set(QUARTZ_LIB_COESITE_HEADER_DIR "/include/quartz/renderer/coesite")

add_custom_target(quartz_renderer_coesite_headers ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_COESITE_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_RENDERER_COESITE_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_LIB_COESITE_HEADER_DIR}"
    DEPENDS ${QUARTZ_RENDERER_COESITE_HEADER_FILES}
    COMMENT "Copying coesite headers"
    VERBATIM
)

add_dependencies(quartz_renderer_coesite quartz_renderer_coesite_headers)
