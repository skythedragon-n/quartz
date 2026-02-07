cmake_minimum_required(VERSION 4.0)
project(quartz)

set(CMAKE_CXX_STANDARD 23)

add_library(quartz_renderer)

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

add_executable(quartz src/renderer/main.cpp)
