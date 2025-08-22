cmake_minimum_required(VERSION 3.31.6)
project(Codesh_Compiler CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Codesh_Compiler
        src/main.cpp
        src/token/token_group.h
        src/lexer/regex.h
        src/parser/command_parser.cpp
        src/parser/command_parser.h
        src/lexer/regex.cpp
        src/lexer/group_name.h
        src/token/token.h
        src/token/token.cpp)


# sudo dnf install boost-regex boost-devel

find_package(Boost REQUIRED regex)
target_link_libraries(Codesh_Compiler Boost::boost)
