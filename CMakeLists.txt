cmake_minimum_required(VERSION 3.24)
project(tarracsh)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_DEFAULT 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexperimental-library")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstd++")
endif()
#message( FATAL_ERROR "${CMAKE_CXX_COMPILER_ID}" )

#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#
#message( FATAL_ERROR "exit" )


#[[
 Tarracsh target
]]


file(GLOB_RECURSE TARRACSH_SOURCES
        "src/**/*.cpp"
        "src/app/server/digest/proto/*.cc"
        )

set(TARRACSH_EXCLUDE_PATTERNS "*/src/test/*" "*/test/src*")

foreach (EXCLUDE_PATTERN ${TARRACSH_EXCLUDE_PATTERNS})
   file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
   list(REMOVE_ITEM TARRACSH_SOURCES ${EXCLUDED_FILES})
endforeach ()
# message(STATUS "TARRACSH_SOURCES: ${TARRACSH_SOURCES}")

add_executable(tarracsh ${TARRACSH_SOURCES}
        src/domain/graph/ClassfileProcessor.cpp
        src/domain/graph/ClassFileProcessor.h)
target_include_directories(tarracsh PUBLIC "src")

find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(tarracsh PRIVATE ICU::uc ICU::io)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")


find_package(antlr4-runtime REQUIRED CONFIG)
#target_link_libraries(tarracsh PRIVATE antlr4_shared)
target_link_libraries(tarracsh PRIVATE antlr4_static)
# message( FATAL_ERROR "antlr4 dir: ${antlr4-runtime_DIR}" )
target_include_directories(tarracsh PRIVATE ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/antlr4-runtime)
#target_link_libraries(tarracsh PRIVATE ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/antlr4-runtime.lib)
#

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE CLI11::CLI11)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(inja CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE pantor::inja)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE unofficial-sodium::sodium)

find_package(libzippp CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE libzippp::libzippp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    find_package(gperftools CONFIG REQUIRED)
#target_link_libraries(tarracsh PRIVATE  gperftools::libtcmalloc_minimal)
endif()

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(tarracsh PRIVATE ${OOF_INCLUDE_DIRS})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(tarracsh PRIVATE fmt::fmt-header-only)

# get_cmake_property(_variableNames VARIABLES)
# foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

# message( FATAL_ERROR "exit" )





