
##Build instructions

Install gcc13 - you may need to build gcc-13 if you are in an older version of ubuntu/linux
Install cmake
Install ninja-build

structure
repos
	-tarracsh
	-vcpkg

git clone git@github.com:kapabl/tarracsh.git
git clone https://github.com/microsoft/vcpkg.git

cd tarracsh/cfdiff
./build-all.sh