
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
src/id_sys.hpp
src/core_errors.hpp
src/core_errors.hpp
src/core_errors.hpp
src/AnimatedLayer.cpp
src/AnimatedLayer.hpp
src/FrameContainer.cpp
src/FrameContainer.hpp
src/Instance.hpp
src/Frame.hpp
src/Frame.cpp
src/overloads.hpp
src/panic.hpp
src/panic.cpp
)

set(QUARTZ_CORE_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/quartz/core")

target_include_directories(quartz_core PUBLIC include)

set(QUARTZ_CORE_HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AnimatedLayer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AnimFile.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core_errors.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Drawing.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/drawing_utils.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Frame.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FrameContainer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/id_sys.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Instance.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Library.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/LibraryFolder.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Stroke.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Symbol.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/overloads.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/panic.hpp"
)

add_custom_target(quartz_core_headers ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}${QUARTZ_CORE_HEADER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${QUARTZ_CORE_HEADER_FILES}
        "${QUARTZ_CORE_HEADER_DIR}"
    DEPENDS ${QUARTZ_CORE_HEADER_FILES}
    COMMENT "Copying core headers"
    VERBATIM
)

add_dependencies(quartz_core quartz_core_headers)
