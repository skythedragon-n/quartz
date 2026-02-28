
add_library(quartz_core
src/Context.cpp
src/Context.hpp
src/AnimFile.cpp
src/AnimFile.hpp
src/Library.cpp
src/Library.hpp
src/LibraryFolder.cpp
src/LibraryFolder.hpp
src/Symbol.cpp
src/Symbol.hpp
src/Drawing.cpp
src/Drawing.hpp
src/Stroke.cpp
src/Stroke.hpp
src/drawing_utils.hpp
src/id_sys.cpp
src/id_sys.hpp
src/core_errors.hpp
src/FrameContainer.cpp
src/FrameContainer.hpp
)

set(CORE_HEADER_STAMP "${CMAKE_CURRENT_SOURCE_DIR}/header.stamp")

set(QUARTZ_CORE_HEADER_DIR "/include/quartz/core")

target_include_directories(quartz_core PUBLIC include)

set(QUARTZ_CORE_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AnimFile.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core_errors.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Drawing.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/drawing_utils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FrameContainer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/id_sys.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Library.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/LibraryFolder.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Stroke.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Symbol.hpp"
)

add_custom_command(
    OUTPUT "${CORE_HEADER_STAMP}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_CORE_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_CORE_HEADER_FILES}
        "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_CORE_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${CORE_HEADER_STAMP}"
    DEPENDS ${HEADER_FILES}
    COMMENT "Copying core headers"
    VERBATIM
)

add_custom_target(quartz_core_headers
    DEPENDS "${CORE_HEADER_STAMP}"
)

add_dependencies(quartz_core quartz_core_headers)
