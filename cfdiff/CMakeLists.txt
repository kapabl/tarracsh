cmake_minimum_required(VERSION 3.24)
project(cfdiff)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
 cfdiff target
]]

message( STATUS "source dir: ${CMAKE_CURRENT_SOURCE_DIR}" )

file(GLOB_RECURSE CFDIFF_SOURCES src/**/*.cpp)
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Main.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app/App.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/domain/jar/tasks/DbBasedTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/domain/jar/tasks/DigestTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/domain/jar/tasks/GraphTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/app/Analyzer.cpp")


set(CFDIFF_EXCLUDE_PATTERNS
        "src/app/server/*"
        "src/app/commands/*"
        "src/domain/db/*"
        "src/domain/digest/*"
        "src/infrastructure/db/*"
        )

foreach (EXCLUDE_PATTERN ${CFDIFF_EXCLUDE_PATTERNS})
    file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
    list(REMOVE_ITEM CFDIFF_SOURCES ${EXCLUDED_FILES})
endforeach ()

message(STATUS "CFDIFF_SOURCES: ${CFDIFF_SOURCES}")

add_executable(cfdiff ${CFDIFF_SOURCES})
target_include_directories(cfdiff PUBLIC "src")

find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(cfdiff PRIVATE ICU::uc ICU::io)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(cfdiff PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE CLI11::CLI11)

find_package(inja CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE pantor::inja)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE unofficial-sodium::sodium)

find_package(libzippp CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE libzippp::libzippp)

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(cfdiff PRIVATE ${OOF_INCLUDE_DIRS})





