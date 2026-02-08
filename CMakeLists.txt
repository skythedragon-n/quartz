cmake_minimum_required(VERSION 4.0)
project(quartz)

set(CMAKE_CXX_STANDARD 23)

add_library(quartz_renderer
src/renderer/Context.cpp
src/renderer/Context.hpp
src/renderer/AnimFile.cpp
src/renderer/AnimFile.hpp
src/renderer/Library.cpp
src/renderer/Library.hpp
src/renderer/LibraryFolder.cpp
src/renderer/LibraryFolder.hpp
src/renderer/Symbol.cpp
src/renderer/Symbol.hpp
src/renderer/Drawing.cpp
src/renderer/Drawing.hpp
src/renderer/Stroke.cpp
src/renderer/Stroke.hpp
src/renderer/utils.hpp
)

#           DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# make sure they exist
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/bx/makefile")
	message(FATAL_ERROR "The bx submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/bimg/makefile")
	message(FATAL_ERROR "The bimg submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/bgfx/makefile")
	message(FATAL_ERROR "The bgfx submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/pugixml/CMakeLists.txt")
    message(FATAL_ERROR "The pugixml submodule was not downloaded! Please update submodules and try again.")
endif()

add_library(vg-renderer STATIC
    "submodules/vg-renderer/src/vg.cpp"
    "submodules/vg-renderer/src/stroker.cpp"
    "submodules/vg-renderer/src/path.cpp"
    "submodules/vg-renderer/src/vg_util.cpp"
    "submodules/vg-renderer/src/libs/fontstash.cpp"
    "submodules/vg-renderer/src/libs/stb_truetype.cpp"

    "submodules/vg-renderer/src/libtess2/bucketalloc.c"
    "submodules/vg-renderer/src/libtess2/dict.c"
    "submodules/vg-renderer/src/libtess2/geom.c"
    "submodules/vg-renderer/src/libtess2/mesh.c"
    "submodules/vg-renderer/src/libtess2/priorityq.c"
    "submodules/vg-renderer/src/libtess2/sweep.c"
    "submodules/vg-renderer/src/libtess2/tess.c"
    )
target_include_directories(vg-renderer PUBLIC "submodules/vg-renderer/include/")

target_include_directories(vg-renderer PUBLIC
    submodules/vg-renderer/src
)

target_include_directories(vg-renderer PUBLIC
    submodules/bgfx/include
    submodules/bx/include
)

add_subdirectory(submodules/pugixml)

if(TARGET pugixml::pugixml)
    target_link_libraries(quartz_renderer pugixml::pugixml)
elseif(TARGET pugixml)
    target_link_libraries(quartz_renderer pugixml)
elseif(TARGET pugixml-static)
    target_link_libraries(quartz_renderer pugixml-static)
else()
    message(FATAL_ERROR "pugixml CMake target not found after add_subdirectory(submodules/pugixml).")
endif()

if(UNIX)
    #include dirs
    target_include_directories(quartz_renderer
	    PRIVATE
		submodules/bgfx/include
		submodules/bx/include
		submodules/bimg/include
    )

    target_include_directories(quartz_renderer
	    PRIVATE
		submodules/bgfx/3rdparty/
		submodules/bgfx/examples/common
	)

    #LINK LIBS
    target_link_libraries(quartz_renderer GL X11 ${CMAKE_DL_LIBS} pthread)

    #BGFX LINKS LOGIC
    target_link_directories(quartz_renderer PRIVATE ./submodules/bgfx/.build/linux64_gcc/bin/)

    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/bgfx/.build/linux64_gcc/bin/libbgfxDebug.a")
        message(FATAL_ERROR "The bgfx submodule was not built as debug! Go to bgfx folder and run: make linux-debug64")
        endif()
	    target_link_libraries(quartz_renderer
		    bgfxDebug
		    bimgDebug
		    bxDebug
		    example-glueDebug
		    example-commonDebug
		    bimg_decodeDebug
	    )
        elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submodules/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a")
        message(FATAL_ERROR "The bgfx submodule was not built as release! Go to bgfx folder and run: make linux-release64")
    endif()
	target_link_libraries(quartz_renderer
		example-glueRelease
		example-commonRelease
		bimg_decodeRelease
		bgfxRelease
		bimgRelease
		bxRelease
	)
    endif()
else()
	message(FATAL_ERROR "No Windows build preparations. Sorry")
endif()

add_executable(amethyst
src/amethyst/main.cpp)
target_link_libraries(quartz_renderer pugixml)
#target_link_libraries(quartz_renderer vg-renderer)
target_link_libraries(quartz_renderer simple-svg)

add_executable(quartz_test
src/test/main.cpp)
