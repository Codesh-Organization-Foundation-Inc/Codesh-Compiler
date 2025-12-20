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
        src/parser/ast/impl/binary_ast_node.cpp
        src/parser/ast/impl/binary_ast_node.h
        src/parser/ast/method/operation/while_ast_node.cpp
        src/parser/ast/method/operation/while_ast_node.h
        src/parser/ast/impl/ir_emitting_ast_node.h
        src/parser/ast/local_variable_declaration_ast_node.cpp
        src/parser/ast/local_variable_declaration_ast_node.h
        src/parser/ast/compilation_unit_ast_node.cpp
        src/parser/ast/compilation_unit_ast_node.h
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
        src/semantic_analyzer/type_decl/collect.h
        src/semantic_analyzer/type_decl/collect.cpp
        src/semantic_analyzer/method_decl/collect.h
        src/semantic_analyzer/method_decl/collect.cpp
        src/semantic_analyzer/symbol_table/symbol.h
        src/parser/ast/method/constructor_declaration_ast_node.cpp
        src/parser/ast/method/constructor_declaration_ast_node.h
        src/semantic_analyzer/util.cpp
        src/semantic_analyzer/util.h
        src/semantic_analyzer/symbol_table/symbol.cpp
        src/semantic_analyzer/symbol_table/scope.h
        src/semantic_analyzer/symbol_table/symbol.tpp
        src/parser/ast/impl/i_resolvable.h
        src/parser/ast/impl/i_descriptor_emitter.cpp
        src/parser/ast/impl/i_resolvable.tpp
        src/semantic_analyzer/method_decl/resolve.cpp
        src/semantic_analyzer/method_decl/resolve.h
        src/semantic_analyzer/method_decl/resolve_aliases.cpp
        src/semantic_analyzer/method_decl/resolve_aliases.h
        src/blasphemy/blasphemy_collector.h
        src/blasphemy/blasphemy_collector.cpp
        src/output/ir/instruction.cpp
        src/output/ir/instruction.h
        src/output/ir/code_block.cpp
        src/output/ir/code_block.h
        src/output/ir/instruction.cpp
        src/output/ir/instruction.h
        src/parser/ast/method/operation/return_ast_node.cpp
        src/parser/ast/method/operation/return_ast_node.h
        src/parser/ast/operator/boolean/not_operator_ast_node.h
        src/parser/ast/operator/boolean/not_operator_ast_node.cpp
        src/parser/ast/method/operation/super_call_ast_node.cpp
        src/parser/ast/method/operation/super_call_ast_node.h
        src/parser/ast/var_reference/evaluable_ast_node.tpp
        src/parser/ast/var_reference/evaluable_ast_node.h
        src/parser/ast/var_reference/value_ast_node.h
        src/parser/ast/method/operation/method_call_ast_node.cpp
        src/parser/ast/method/operation/method_call_ast_node.h
        src/defenition/fully_qualified_class_name.cpp
        src/defenition/fully_qualified_class_name.h
        src/parser/ast/var_reference/value_ast_node.cpp
        src/blasphemy/details.h
        src/blasphemy/details.cpp
        src/semantic_analyzer/method_call/resolve.cpp
        src/semantic_analyzer/method_call/resolve.h
        src/blasphemy/blasphemy_consumer.h
        src/blasphemy/blasphemy_consumer.cpp
        src/semantic_analyzer/type_decl/resolve.cpp
        src/semantic_analyzer/type_decl/resolve.h
        src/semantic_analyzer/semantic_context.h
        src/semantic_analyzer/semantic_context.cpp
        src/semantic_analyzer/type_decl/resolve_aliases.cpp
        src/semantic_analyzer/type_decl/resolve_aliases.h
        src/parser/ast/method/util.cpp
        src/parser/ast/method/util.h
        src/semantic_analyzer/builtins.cpp
        src/semantic_analyzer/builtins.h
        src/parser/ast/var_reference/error_value_ast_node.cpp
        src/parser/ast/var_reference/error_value_ast_node.h
)

# sudo dnf install boost-regex boost-devel
find_package(Boost REQUIRED COMPONENTS regex headers)

target_link_libraries(Codesh_Compiler Boost::boost Boost::regex)

# sudo dnf install libicu-devel
find_package(ICU REQUIRED COMPONENTS i18n uc data)
include_directories(${ICU_INCLUDE_DIRS})

target_link_libraries(Codesh_Compiler
        ${ICU_LIBRARIES} icui18n
        icuuc
        icudata
)

# fmt
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 12.1.0
)

FetchContent_MakeAvailable(fmt)
target_link_libraries(Codesh_Compiler fmt::fmt)
