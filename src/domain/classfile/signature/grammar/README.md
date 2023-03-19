## Install antlr4 and run this in the directory to generate c++ files

antlr4 -Dlanguage=Cpp  -package kapa::tarracsh::domain::classfile::signature::parser JvmSignature.g4

Installing antlr4 with vcpkg is required too