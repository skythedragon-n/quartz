
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
src/drawingutils.hpp
src/libraryutils.cpp
src/libraryutils.hpp
)

target_include_directories(quartz_core PUBLIC include)
