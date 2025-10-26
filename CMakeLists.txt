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
        src/parser/ast/impl/typed_ast_node.h
        src/parser/ast/variable_declaration_ast_node.cpp
        src/parser/ast/variable_declaration_ast_node.h
        src/parser/ast/compilation_unit_ast_node.cpp
        src/parser/ast/compilation_unit_ast_node.h
        src/parser/ast/impl/typed_ast_node.h
        src/parser/ast/operator/math/addition_operator_ast_node.cpp
        src/parser/ast/operator/math/addition_operator_ast_node.h
        src/parser/ast/operator/math/subtraction_operator_ast_node.cpp
        src/parser/ast/operator/math/subtraction_operator_ast_node.h
        src/parser/ast/operator/math/multiplication_operator_ast_node.cpp
        src/parser/ast/operator/math/multiplication_operator_ast_node.h
        src/parser/ast/operator/math/division_operator_ast_node.cpp
        src/parser/ast/operator/math/division_operator_ast_node.h
        src/parser/ast/operator/math/modulu_operator_ast_node.cpp
        src/parser/ast/operator/math/modulu_operator_ast_node.h
        src/parser/ast/operator/boolean/equals_operator_ast_node.cpp
        src/parser/ast/operator/boolean/equals_operator_ast_node.h
        src/parser/ast/operator/boolean/not_equals_operator_ast_node.cpp
        src/parser/ast/operator/boolean/not_equals_operator_ast_node.h
        src/parser/ast/operator/boolean/greater_operator_ast_node.cpp
        src/parser/ast/operator/boolean/greater_operator_ast_node.h
        src/parser/ast/operator/boolean/greater_equals_operator_ast_node.cpp
        src/parser/ast/operator/boolean/greater_equals_operator_ast_node.h
        src/parser/ast/operator/boolean/less_operator_ast_node.cpp
        src/parser/ast/operator/boolean/less_operator_ast_node.h
        src/parser/ast/operator/boolean/less_equals_operator_ast_node.cpp
        src/parser/ast/operator/boolean/less_equals_operator_ast_node.h
        src/parser/ast/operator/boolean/and_operator_ast_node.cpp
        src/parser/ast/operator/boolean/and_operator_ast_node.h
        src/parser/ast/operator/boolean/or_operator_ast_node.cpp
        src/parser/ast/operator/boolean/or_operator_ast_node.h
        src/parser/ast/import_declaration_ast_node.cpp
        src/parser/ast/import_declaration_ast_node.h
        src/parser/ast/type_declaration_ast_node.cpp
        src/parser/ast/type_declaration_ast_node.h
        src/parser/ast/impl/unary_ast_node.cpp
        src/parser/ast/impl/unary_ast_node.h
        src/lexer/trie/trie_node.cpp
        src/lexer/trie/trie_node.h
        src/lexer/trie/keyword_info.h
        src/lexer/trie/word_boundary.h
        src/lexer/trie/keywords.cpp
        src/lexer/trie/keywords.h
        src/lexer/trie/trie.cpp
        src/lexer/trie/trie.h)


# sudo dnf install boost-regex boost-devel

find_package(Boost REQUIRED regex)
target_link_libraries(Codesh_Compiler Boost::boost)

# sudo dnf install libicu-devel

find_package(ICU REQUIRED COMPONENTS uc i18n)
include_directories(${ICU_INCLUDE_DIRS})
target_link_libraries(Codesh_Compiler ${ICU_LIBRARIES})
