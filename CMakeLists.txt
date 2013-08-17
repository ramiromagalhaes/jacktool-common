project(jacktool-common)
cmake_minimum_required(VERSION 2.8)



# Debug and release configurations
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -D_DEBUG -Wextra -Wall")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")



# More on find_package command http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
find_package( OpenCV REQUIRED COMPONENTS core imgproc highgui )
find_package( Boost REQUIRED COMPONENTS filesystem system serialization )



# Builds the library
file( GLOB LIB_SOURCES *.cpp )
file( GLOB LIB_HEADERS *.h )
add_library( jacktool-common ${LIB_SOURCES} ${LIB_HEADERS} )
