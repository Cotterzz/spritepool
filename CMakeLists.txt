cmake_minimum_required(VERSION 3.23) #Some version that is not too recent and not too old

project(spritepool) # Project name. This is not related to the executable nor to any filename.

# First argument is the "target name". CMake understands all things in targets, and targets can be 
# - Executables
# - Static libraries
# - Shared libraries
# - "Interface libraries", i.e. header-only libraries
add_executable(main main.c pool.c) # This here is a simple executable consisting of two files

target_link_libraries(main PUBLIC raylib) # We always want to link raylib 

if(WIN32)
    target_link_libraries(main PUBLIC gdi32 winmm)
else()
    target_link_libraries(main PUBLIC GL m pthread dl rt X11)
endif()

# create a directory build with mkdir build
# then cd build
# then you run cmake ..
# this will default to generating Unix Makefiles
# if you run rm -rf * in build, everything including cmake's cache is cleared
# however, make clean will also work, because cmake will emit a "clean" target that performs what's intended
# clean all the binary stuff
# but make clean will not reset cmake's configuration cache
