cmake_minimum_required(VERSION 3.31.6)
project(Codesh_Compiler C)

set(CMAKE_C_STANDARD 11)

add_executable(Codesh_Compiler
        src/main.c
        src/token/token.c
        src/token/token.h
        src/token/token_type.h
        src/token/token_type.h)
