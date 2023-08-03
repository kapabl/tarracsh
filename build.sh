#!/bin/bash

export VCPKG_ROOT="$HOME/repos/vcpkg"

export BUILD_TYPE="${BUILD_TYPE:-Release}"
export BUILD_DIR="cmake-build-${BUILD_TYPE,,}"

echo Configuring $BUILD_TYPE

cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DVCPKG_ROOT=$VCPKG_ROOT -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -G Ninja -S . -B $BUILD_DIR

echo Building $BUILD_TYPE...
cmake --build $BUILD_DIR --target tarracsh -j 2
