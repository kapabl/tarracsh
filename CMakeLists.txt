cmake_minimum_required(VERSION 3.24)
project(tarracsh)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_DEFAULT 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)

include(CTest)

function(tarracsh_apply_coverage target)
    if(ENABLE_COVERAGE)
        if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            message(FATAL_ERROR "Coverage instrumentation requires GCC or Clang.")
        endif()
        target_compile_options(${target} PRIVATE -O0 -g --coverage)
        target_link_options(${target} PRIVATE --coverage)
    endif()
endfunction()

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

set(TARRACSH_MAIN_SOURCE "src/app/Main.cpp")
list(FILTER TARRACSH_SOURCES EXCLUDE REGEX ".*/Main.cpp$")

set(TARRACSH_EXCLUDE_PATTERNS "*/src/test/*" "*/test/src*")

foreach (EXCLUDE_PATTERN ${TARRACSH_EXCLUDE_PATTERNS})
   file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
   list(REMOVE_ITEM TARRACSH_SOURCES ${EXCLUDED_FILES})
endforeach ()
# message(STATUS "TARRACSH_SOURCES: ${TARRACSH_SOURCES}")

add_library(tarracsh_core STATIC ${TARRACSH_SOURCES})
set_target_properties(tarracsh_core PROPERTIES OUTPUT_NAME tarracsh_core)
target_include_directories(tarracsh_core PUBLIC "src")

add_executable(tarracsh ${TARRACSH_MAIN_SOURCE})
target_link_libraries(tarracsh PRIVATE tarracsh_core)

find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(tarracsh_core PRIVATE ICU::uc ICU::io)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")

if(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS)
    target_include_directories(tarracsh_core PUBLIC ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})
endif()


find_package(antlr4-runtime REQUIRED CONFIG)
#target_link_libraries(tarracsh PRIVATE antlr4_shared)
target_link_libraries(tarracsh_core PRIVATE antlr4_static)
# message( FATAL_ERROR "antlr4 dir: ${antlr4-runtime_DIR}" )
target_include_directories(tarracsh_core PUBLIC ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/antlr4-runtime)
#target_link_libraries(tarracsh PRIVATE ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/antlr4-runtime.lib)
#

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE CLI11::CLI11)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(inja CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE pantor::inja)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE unofficial-sodium::sodium)

find_package(libzippp CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE libzippp::libzippp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    find_package(gperftools CONFIG REQUIRED)
#target_link_libraries(tarracsh PRIVATE  gperftools::libtcmalloc_minimal)
endif()

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(tarracsh_core PUBLIC ${OOF_INCLUDE_DIRS})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(tarracsh_core PRIVATE fmt::fmt-header-only)

tarracsh_apply_coverage(tarracsh_core)
tarracsh_apply_coverage(tarracsh)

if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    file(GLOB_RECURSE TARRACSH_TEST_SOURCES "test/src/*.cpp")
    add_executable(tarracsh_tests ${TARRACSH_TEST_SOURCES})
    target_link_libraries(tarracsh_tests PRIVATE tarracsh_core GTest::gtest_main)
    target_include_directories(tarracsh_tests PRIVATE "src")
    tarracsh_apply_coverage(tarracsh_tests)
    include(GoogleTest)
    gtest_discover_tests(tarracsh_tests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

    if(ENABLE_COVERAGE)
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)
        if(LCOV_PATH AND GENHTML_PATH)
            set(COVERAGE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/coverage")
            add_custom_target(coverage
                COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_DIR}
                COMMAND ${LCOV_PATH} --directory ${CMAKE_BINARY_DIR} --zerocounters
                COMMAND ${LCOV_PATH} --directory ${CMAKE_BINARY_DIR} --capture --initial --output-file ${COVERAGE_OUTPUT_DIR}/baseline.info
                COMMAND ctest --output-on-failure
                COMMAND ${LCOV_PATH} --directory ${CMAKE_BINARY_DIR} --capture --output-file ${COVERAGE_OUTPUT_DIR}/coverage.info
                COMMAND ${LCOV_PATH} --add-tracefile ${COVERAGE_OUTPUT_DIR}/baseline.info --add-tracefile ${COVERAGE_OUTPUT_DIR}/coverage.info --output-file ${COVERAGE_OUTPUT_DIR}/lcov.info
                COMMAND ${GENHTML_PATH} ${COVERAGE_OUTPUT_DIR}/lcov.info --output-directory ${COVERAGE_OUTPUT_DIR}/html
                BYPRODUCTS
                    ${COVERAGE_OUTPUT_DIR}/baseline.info
                    ${COVERAGE_OUTPUT_DIR}/coverage.info
                    ${COVERAGE_OUTPUT_DIR}/lcov.info
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                DEPENDS tarracsh_tests
            )
        else()
            message(WARNING "Coverage target requested but lcov/genhtml not found.")
        endif()
    endif()
endif()

# get_cmake_property(_variableNames VARIABLES)
# foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

# message( FATAL_ERROR "exit" )


