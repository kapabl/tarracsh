cd src\server\proto
del *.h
del *.cc
protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=D:\dev\kapa\repos\vcpkg\buildtrees\grpc\x64-windows-rel\grpc_cpp_plugin.exe *.proto
protoc -I . --cpp_out=. *.proto
cd ..\..\..\