cmake_minimum_required(VERSION 3.24)
project(tarracsh)

#set(CMAKE_GENERATOR_PLATFORM x64)
#set(CMAKE_CXX_FLAGS "-m64")
set(CMAKE_CXX_STANDARD 20)

#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#
#message( FATAL_ERROR "exit" )


file(GLOB_RECURSE CFDIFF_SOURCES cfdiff/src/**/*.cpp)
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/app/Main.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/app/App.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/DbBasedTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/DigestTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/GraphTask.cpp")

set(EXCLUDE_PATTERNS
        "*/src/app/server/*"
        "*/src/app/commands/*"
        "*/src/domain/db/*"
        "*/src/domain/digest/*"
        "*/src/infrastructure/db/*"
        )

foreach (EXCLUDE_PATTERN ${EXCLUDE_PATTERNS})
    file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
    list(REMOVE_ITEM CFDIFF_SOURCES ${EXCLUDED_FILES})
endforeach ()

list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/app/Main.cpp")

message(STATUS "CFDIFF_SOURCES: ${CFDIFF_SOURCES}")


add_executable(cfdiff ${CFDIFF_SOURCES})
target_include_directories(cfdiff PUBLIC "cfdiff/src")


find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(cfdiff PRIVATE ICU::uc ICU::io)


#find_package(Boost REQUIRED COMPONENTS interprocess )
#target_link_libraries(cfdiff PRIVATE Boost::boost Boost::interprocess)
#find_package(Boost REQUIRED)#
#include_directories(${Boost_INCLUDE_DIRS})
#target_link_libraries(cfdiff PRIVATE Boost::interprocess)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(cfdiff PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

# this is heuristically generated, and may not be correct
find_package(antlr4-runtime REQUIRED CONFIG)
target_link_libraries(cfdiff PRIVATE antlr4_shared)
#add_dependencies(cfdiff antlr4_shared)


#catch2 provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

#cli11 provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE CLI11::CLI11)

#grpc provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(gRPC CONFIG REQUIRED)
# note: 7 additional targets are not displayed.
target_link_libraries(cfdiff PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

#find_package(modules CONFIG REQUIRED)
#target_link_libraries(cfdiff PRIVATE re2::re2 c-ares::cares)

#inja provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(inja CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE pantor::inja)

#libsodium provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE unofficial-sodium::sodium)

#libzippp provides CMake targets:

# this is heuristically generated, and may not be correct
find_package(libzippp CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE libzippp::libzippp)

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(cfdiff PRIVATE ${OOF_INCLUDE_DIRS})





