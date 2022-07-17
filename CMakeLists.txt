cmake_minimum_required(VERSION 3.13)
project(tarrash)
set(CMAKE_CXX_STANDARD 20)


include(conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCES
        "src/*.h"
        "src/*.cpp"
        )

# Add main.cpp file of project root directory as source file
#set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(tarrash ${SOURCES})