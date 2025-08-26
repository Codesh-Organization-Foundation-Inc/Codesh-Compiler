cmake_minimum_required(VERSION 3.31.6)
project(Codesh_Compiler CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Codesh_Compiler
        src/main.cpp
        src/lexer/regex.h
        src/parser/command_parser.cpp
        src/parser/command_parser.h
        src/lexer/regex.cpp
        src/token/token.h
        src/token/token.cpp
        src/parser/parser.h
        src/parser/parser.cpp)


# sudo dnf install boost-regex boost-devel

find_package(Boost REQUIRED regex)
target_link_libraries(Codesh_Compiler Boost::boost)
