
add_library(quartz_renderer_coesite
src/Scene.cpp
src/Scene.hpp
src/CoesiteRoot.cpp
src/CoesiteRoot.hpp
src/id_sys.hpp
src/Node.cpp
src/Node.hpp
src/Stroke.cpp
src/Stroke.hpp
)

target_include_directories(quartz_renderer_coesite PUBLIC include)

set(QUARTZ_RENDERER_COESITE_HEADER_FILES
"${CMAKE_CURRENT_SOURCE_DIR}/src/Scene.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/CoesiteRoot.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Node.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Stroke.hpp"
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
