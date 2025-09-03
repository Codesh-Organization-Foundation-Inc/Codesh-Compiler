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
        src/parser/parser.cpp
        src/lexer/tokenizer.h
        src/lexer/tokenizer.cpp
        src/parser/ast/impl/ast_node.cpp
        src/parser/ast/impl/ast_node.h
        src/parser/ast/block_ast_node.cpp
        src/parser/ast/block_ast_node.h
        src/parser/ast/impl/binary_ast_node.tpp
        src/parser/ast/impl/binary_ast_node.h
        src/parser/ast/node_type.h
        src/parser/ast/while_ast_node.cpp
        src/parser/ast/while_ast_node.h
        src/parser/ast/impl/typed_ast_node.h)


# sudo dnf install boost-regex boost-devel

find_package(Boost REQUIRED regex)
target_link_libraries(Codesh_Compiler Boost::boost)

# sudo dnf install libicu-devel

find_package(ICU REQUIRED COMPONENTS uc i18n)
include_directories(${ICU_INCLUDE_DIRS})
target_link_libraries(Codesh_Compiler ${ICU_LIBRARIES})
