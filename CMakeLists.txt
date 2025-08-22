cmake_minimum_required(VERSION 3.31.6)
project(Codesh_Compiler CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Codesh_Compiler
        src/main.cpp
        src/token/token_type.h
        src/lexer/regex_helper.h
        src/lexer/regex_helper.cpp
        src/parser/command_parser.cpp)
