cmake_minimum_required(VERSION 3.24)
project(tarracsh)

set(CMAKE_CXX_STANDARD 20)

#get_cmake_property(_variableNames VARIABLES)
#foreach(_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#
#message( FATAL_ERROR "exit" )

#[[
 cfdiff target
]]

file(GLOB_RECURSE CFDIFF_SOURCES cfdiff/src/**/*.cpp)
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/app/Main.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/app/App.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/DbBasedTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/DigestTask.cpp")
list(REMOVE_ITEM CFDIFF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cfdiff/src/domain/jar/tasks/GraphTask.cpp")

set(CFDIFF_EXCLUDE_PATTERNS
        "*/src/app/server/*"
        "*/src/app/commands/*"
        "*/src/domain/db/*"
        "*/src/domain/digest/*"
        "*/src/infrastructure/db/*"
        )

foreach (EXCLUDE_PATTERN ${CFDIFF_EXCLUDE_PATTERNS})
    file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
    list(REMOVE_ITEM CFDIFF_SOURCES ${EXCLUDED_FILES})
endforeach ()

add_executable(cfdiff ${CFDIFF_SOURCES})
target_include_directories(cfdiff PUBLIC "cfdiff/src")

find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(cfdiff PRIVATE ICU::uc ICU::io)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(cfdiff PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(antlr4-runtime REQUIRED CONFIG)
target_link_libraries(cfdiff PRIVATE antlr4_shared)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

find_package(CLI11 CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE CLI11::CLI11)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(inja CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE pantor::inja)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE unofficial-sodium::sodium)

find_package(libzippp CONFIG REQUIRED)
target_link_libraries(cfdiff PRIVATE libzippp::libzippp)

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(cfdiff PRIVATE ${OOF_INCLUDE_DIRS})


#[[
 Tarracsh target
]]


file(GLOB_RECURSE TARRACSH_SOURCES
        "src/**/*.cpp"
        "src/app/server/digest/proto/*.cc"
        )


#set(TARRACSH_EXCLUDE_PATTERNS "*/src/test/*" "*/test/src*")
#
#foreach (EXCLUDE_PATTERN ${TARRACSH_EXCLUDE_PATTERNS})
#    file(GLOB_RECURSE EXCLUDED_FILES ${EXCLUDE_PATTERN})
#    list(REMOVE_ITEM TARRACSH_SOURCES ${EXCLUDED_FILES})
#endforeach ()
message(STATUS "TARRACSH_SOURCES: ${TARRACSH_SOURCES}")

add_executable(tarracsh ${TARRACSH_SOURCES})
target_include_directories(tarracsh PUBLIC "src")

find_package(ICU REQUIRED COMPONENTS uc io)
target_link_libraries(tarracsh PRIVATE ICU::uc ICU::io)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(tarracsh PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(antlr4-runtime REQUIRED CONFIG)
target_link_libraries(tarracsh PRIVATE antlr4_shared)

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

find_package(gperftools CONFIG REQUIRED)
#target_link_libraries(tarracsh PRIVATE  gperftools::libtcmalloc_minimal)

find_path(OOF_INCLUDE_DIRS "oof.h")
target_include_directories(tarracsh PRIVATE ${OOF_INCLUDE_DIRS})





