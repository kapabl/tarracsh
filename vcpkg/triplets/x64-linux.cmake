message( STATUS "in triplet")
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "/home/kapa/repos/tarracsh/vcpkg/toolchains/toolchain-linux.cmake")
message( STATUS "here:${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")

