cmake_minimum_required(VERSION 3.31.6)
project(Codesh_Compiler CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(Codesh_Compiler
        src/main.cpp
        src/lexer/regex.h
        src/command_parser.cpp
        src/command_parser.h
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
        src/parser/ast/impl/binary_ast_node.cpp
        src/parser/ast/impl/binary_ast_node.h
        src/parser/ast/while_ast_node.cpp
        src/parser/ast/while_ast_node.h
        src/parser/ast/impl/ir_convertable_ast_node.h
        src/parser/ast/local_variable_declaration_ast_node.cpp
        src/parser/ast/local_variable_declaration_ast_node.h
        src/parser/ast/compilation_unit_ast_node.cpp
        src/parser/ast/compilation_unit_ast_node.h
        src/parser/ast/impl/ir_convertable_ast_node.h
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
        src/parser/ast/impl/unary_ast_node.cpp
        src/parser/ast/impl/unary_ast_node.h
        src/lexer/trie/trie_node.cpp
        src/lexer/trie/trie_node.h
        src/lexer/trie/keyword_info.h
        src/lexer/trie/word_boundary.h
        src/lexer/trie/keywords.cpp
        src/lexer/trie/keywords.h
        src/lexer/trie/trie.cpp
        src/lexer/trie/trie.h
        src/parser/ast/impl/unary_ast_node.h
        src/defenition/basad_type.h
        src/parser/ast/type_declaration/class_declaration_ast_node.cpp
        src/parser/ast/type_declaration/attributes_ast_node.cpp
        src/parser/ast/type_declaration/attributes_ast_node.h
        src/parser/ast/type_declaration/type_declaration_ast_node.cpp
        src/parser/ast/type_declaration/type_declaration_ast_node.h
        src/defenition/visibility.h
        src/defenition/visibility.cpp
        src/output/jvm_target/defs/class_file.h
        src/output/jvm_target/defs/cp_info.h
        src/output/jvm_target/defs/methods_info_entry.h
        src/output/jvm_target/defs/attribute_info_entry.h
        src/defenition/visibility.cpp
        src/parser/compilation_unit_parser.h
        src/parser/compilation_unit_parser.cpp
        src/parser/util.cpp
        src/parser/util.h
        src/parser/import_parser.cpp
        src/parser/import_parser.h
        src/parser/type/class/class_parser.cpp
        src/parser/type/class/class_parser.h
        src/parser/type/type_parser.cpp
        src/parser/type/type_parser.h
        src/util.cpp
        src/util.h
        src/parser/ast/method/method_declaration_ast_node.cpp
        src/parser/ast/method/method_declaration_ast_node.h
        src/parser/ast/type/type_ast_node.h
        src/parser/ast/type/custom_type_ast_node.cpp
        src/parser/ast/type/custom_type_ast_node.h
        src/parser/ast/type/primitive_type_ast_node.cpp
        src/parser/ast/type/primitive_type_ast_node.h
        src/defenition/primitive_type.h
        src/parser/ast/local_variable_declaration_ast_node.cpp
        src/parser/ast/local_variable_declaration_ast_node.h
        src/parser/ast/type/type_ast_node.cpp
        src/parser/ast/impl/i_descriptor_emitter.h
        src/test.cpp
        src/test.h
        src/output/jvm_target/class_file_builder.cpp
        src/output/jvm_target/class_file_builder.h
        src/output/jvm_target/defs/attribute_info_entry.cpp
        src/output/jvm_target/defs/cp_info.cpp
        src/output/jvm_target/defs/cp_info.cpp
        src/defenition/definitions.h
        src/output/jvm_target/constant_pool.cpp
        src/output/jvm_target/constant_pool.h
        src/output/jvm_target/class_file_writer.cpp
        src/output/jvm_target/class_file_writer.h
        src/parser/type/class/method_parser.cpp
        src/parser/type/class/method_parser.h
        src/semantic_analyzer/analyzer.h
        src/semantic_analyzer/analyzer.cpp
        src/semantic_analyzer/symbol_table/symbol_table.h
        src/semantic_analyzer/symbol_table/symbol_table.cpp
        src/semantic_analyzer/passes/check_type_declarations.h
        src/semantic_analyzer/passes/check_type_declarations.cpp
        src/semantic_analyzer/passes/check_methods.h
        src/semantic_analyzer/passes/check_methods.cpp
        src/semantic_analyzer/symbol_table/symbol.h
        src/semantic_analyzer/errors/errors.h
        src/semantic_analyzer/errors/errors.cpp
        src/parser/ast/method/constructor_declaration_ast_node.cpp
        src/parser/ast/method/constructor_declaration_ast_node.h
        src/semantic_analyzer/passes/util.cpp
        src/semantic_analyzer/passes/util.h
        src/semantic_analyzer/symbol_table/symbol.cpp
        src/semantic_analyzer/symbol_table/scope.h
        src/semantic_analyzer/symbol_table/symbol.tpp
)

# sudo dnf install boost-regex boost-devel

find_package(Boost REQUIRED)
target_link_libraries(Codesh_Compiler Boost::boost)

# sudo dnf install libicu-devel

find_package(ICU REQUIRED COMPONENTS uc i18n)
include_directories(${ICU_INCLUDE_DIRS})
target_link_libraries(Codesh_Compiler ${ICU_LIBRARIES})


include(FetchContent)
