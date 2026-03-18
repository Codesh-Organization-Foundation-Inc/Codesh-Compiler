cmake_minimum_required(VERSION 3.31.6)
project(codeshc CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(codeshc
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
        src/lexer/source_file_info.cpp
        src/lexer/source_file_info.h
        src/parser/ast/impl/ast_node.cpp
        src/parser/ast/impl/ast_node.h
        src/parser/ast/impl/binary_ast_node.cpp
        src/parser/ast/impl/binary_ast_node.h
        src/parser/ast/method/operation/block/while_ast_node.cpp
        src/parser/ast/method/operation/block/while_ast_node.h
        src/parser/ast/impl/i_ir_emitter.h
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
        src/parser/ast/type_declaration/error_type_declaration_ast_node.cpp
        src/parser/ast/type_declaration/error_type_declaration_ast_node.h
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
        src/parser/ast/type/widening_cast_ast_node.cpp
        src/parser/ast/type/widening_cast_ast_node.h
        src/defenition/primitive_type.h
        src/parser/ast/local_variable_declaration_ast_node.cpp
        src/parser/ast/local_variable_declaration_ast_node.h
        src/parser/ast/type/type_ast_node.cpp
        src/parser/ast/impl/i_descriptor_emitter.h
        src/output/jvm_target/class_file_builder.cpp
        src/output/jvm_target/class_file_builder.h
        src/output/jvm_target/stack_map_table_builder.cpp
        src/output/jvm_target/stack_map_table_builder.h
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
        src/semantic_analyzer/field_decl/collect.h
        src/semantic_analyzer/field_decl/collect.cpp
        src/semantic_analyzer/field_decl/resolve.h
        src/semantic_analyzer/field_decl/resolve.cpp
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
        src/output/ir/instruction/impl/opcode.h
        src/output/ir/instruction/impl/instruction_container.cpp
        src/output/ir/instruction/impl/instruction_container.h
        src/output/ir/instruction/impl/instruction.cpp
        src/output/ir/instruction/impl/instruction.h
        src/output/ir/instruction/impl/simple_instruction.cpp
        src/output/ir/instruction/impl/simple_instruction.h
        src/output/ir/instruction/impl/typed_instruction.cpp
        src/output/ir/instruction/impl/typed_instruction.h
        src/output/ir/instruction/nop_instruction.cpp
        src/output/ir/instruction/nop_instruction.h
        src/output/ir/instruction/load_instruction.cpp
        src/output/ir/instruction/load_instruction.h
        src/output/ir/instruction/return_instruction.cpp
        src/output/ir/instruction/return_instruction.h
        src/output/ir/instruction/widening_cast_instruction.cpp
        src/output/ir/instruction/widening_cast_instruction.h
        src/output/ir/instruction/invoke_instruction.cpp
        src/output/ir/instruction/invoke_instruction.h
        src/output/ir/instruction/load_int_constant_instruction.cpp
        src/output/ir/instruction/load_int_constant_instruction.h
        src/output/ir/instruction/load_constant_pool_instruction.cpp
        src/output/ir/instruction/load_constant_pool_instruction.h
        src/output/ir/instruction/load_wide_constant_pool_instruction.cpp
        src/output/ir/instruction/load_wide_constant_pool_instruction.h
        src/output/ir/instruction/store_in_local_var_instruction.cpp
        src/output/ir/instruction/store_in_local_var_instruction.h
        src/output/ir/instruction/operator_instruction.cpp
        src/output/ir/instruction/operator_instruction.h
        src/output/ir/instruction/get_static_instruction.cpp
        src/output/ir/instruction/get_static_instruction.h
        src/output/ir/instruction/get_field_instruction.cpp
        src/output/ir/instruction/get_field_instruction.h
        src/output/ir/instruction/put_field_instruction.cpp
        src/output/ir/instruction/put_field_instruction.h
        src/output/ir/instruction/put_static_instruction.cpp
        src/output/ir/instruction/put_static_instruction.h
        src/output/ir/instruction/goto_instruction.cpp
        src/output/ir/instruction/goto_instruction.h
        src/output/ir/instruction/if_instruction.cpp
        src/output/ir/instruction/if_instruction.h
        src/output/ir/instruction/assignment_instruction.cpp
        src/output/ir/instruction/assignment_instruction.h
        src/output/ir/instruction/assignment_from_code_block_instruction.cpp
        src/output/ir/instruction/assignment_from_code_block_instruction.h
        src/output/ir/instruction/assignment_from_constant_instruction.cpp
        src/output/ir/instruction/assignment_from_constant_instruction.h
        src/output/ir/instruction/increment_int_by_constant_instruction.cpp
        src/output/ir/instruction/increment_int_by_constant_instruction.h
        src/output/ir/instruction/marker_instruction.cpp
        src/output/ir/instruction/marker_instruction.h
        src/output/ir/instruction/scope_marker.cpp
        src/output/ir/instruction/scope_marker.h
        src/output/ir/code_block.cpp
        src/output/ir/code_block.h
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
        src/defenition/fully_qualified_name.cpp
        src/defenition/fully_qualified_name.h
        src/blasphemy/details.h
        src/blasphemy/details.cpp
        src/semantic_analyzer/statement/method_call/resolve.cpp
        src/semantic_analyzer/statement/method_call/resolve.h
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
        src/semantic_analyzer/external/class_loader.h
        src/semantic_analyzer/external/class_loader.cpp
        src/parser/ast/var_reference/error_value_ast_node.cpp
        src/parser/ast/var_reference/error_value_ast_node.h
        src/parser/ast/var_reference/variable_reference_ast_node.cpp
        src/parser/ast/var_reference/variable_reference_ast_node.h
        src/parser/value/value_parser.cpp
        src/parser/value/value_parser.h
        src/parser/value/arithmetic_value_parser.cpp
        src/parser/value/arithmetic_value_parser.h
        src/parser/value/boolean_value_parser.cpp
        src/parser/value/boolean_value_parser.h
        src/parser/value/assignment_value_parser.cpp
        src/parser/value/assignment_value_parser.h
        src/parser/value/new_value_parser.cpp
        src/parser/value/new_value_parser.h
        src/parser/value/primitive_value_parser.cpp
        src/parser/value/primitive_value_parser.h
        src/parser/ast/impl/i_constant_pool_emitter.h
        src/parser/ast/operator/math/minus_operator_ast_node.cpp
        src/parser/ast/operator/math/minus_operator_ast_node.h
        src/parser/ast/method/operation/method_operation_ast_node.cpp
        src/parser/ast/method/operation/method_operation_ast_node.h
        src/parser/ast/method/method_scope_ast_node.cpp
        src/parser/ast/method/method_scope_ast_node.h
        src/parser/ast/impl/i_symbolically_linked.h
        src/parser/ast/impl/i_symbolically_linked.tpp
        src/semantic_analyzer/symbol_table/scope.cpp
        src/semantic_analyzer/symbol_table/scope.tpp
        src/semantic_analyzer/symbol_table/symbol_type.h
        src/semantic_analyzer/statement/resolve.cpp
        src/semantic_analyzer/statement/resolve.h
        src/semantic_analyzer/statement/resolve.cpp
        src/semantic_analyzer/statement/variable_reference/resolve.cpp
        src/semantic_analyzer/statement/variable_reference/resolve.h
        src/parser/ast/operator/assignment/assign_operator_ast_node.cpp
        src/parser/ast/operator/assignment/assign_operator_ast_node.h
        src/parser/ast/operator/assignment/assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/addition_assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/addition_assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/subtraction_assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/subtraction_assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/multiplication_assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/multiplication_assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/division_assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/division_assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/modulo_assignment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/modulo_assignment_operator_ast_node.h
        src/parser/ast/operator/assignment/increment_decrement_operator_ast_node.cpp
        src/parser/ast/operator/assignment/increment_decrement_operator_ast_node.h
        src/parser/ast/operator/assignment/increment_operator_ast_node.cpp
        src/parser/ast/operator/assignment/increment_operator_ast_node.h
        src/parser/ast/operator/assignment/decrement_operator_ast_node.cpp
        src/parser/ast/operator/assignment/decrement_operator_ast_node.h
        src/parser/ast/method/operation/block/if_ast_node.cpp
        src/parser/ast/method/operation/block/if_ast_node.h
        src/parser/ast/impl/operator_ast_node.h
        src/parser/ast/method/operation/block/for_ast_node.cpp
        src/parser/ast/method/operation/block/for_ast_node.h
        src/parser/ast/collection/collection_ast_node.h
        src/parser/ast/collection/range_ast_node.cpp
        src/parser/ast/collection/range_ast_node.h
        src/output/jvm_target/defs/stack_map_table.h
        src/output/jvm_target/defs/stack_map_table.cpp
        src/output/ir/condition_block_builder.h
        src/output/ir/condition_block_builder.cpp
        src/parser/ast/operator/boolean/boolean_binary_ast_node.cpp
        src/parser/ast/operator/boolean/boolean_binary_ast_node.h
        src/parser/ast/method/operation/new_ast_node.cpp
        src/parser/ast/method/operation/new_ast_node.h
        src/parser/ast/impl/operator_ast_node.tpp
        src/parser/ast/operator/math/i_math_operator_holder.h
        src/defenition/i_pretty_string_convertable.h
        src/output/ir/util.cpp
        src/output/ir/util.h
        src/parser/value/biblical_numbers_parser.cpp
        src/parser/value/biblical_numbers_parser.h
        src/output/ir/instruction/dup_instruction.cpp
        src/output/ir/instruction/dup_instruction.h
        src/output/ir/instruction/new_instruction.cpp
        src/output/ir/instruction/new_instruction.h
        src/output/ir/instruction/pop_instruction.cpp
        src/output/ir/instruction/pop_instruction.h
        src/parser/ast/type_declaration/field_declaration_ast_node.cpp
        src/parser/ast/type_declaration/field_declaration_ast_node.h
        src/parser/ast/type_declaration/variable_declaration_ast_node.cpp
        src/parser/ast/type_declaration/variable_declaration_ast_node.h
        src/semantic_analyzer/external/jimage_loader.h
        src/semantic_analyzer/external/jimage_loader.cpp
        src/semantic_analyzer/external/util.cpp
        src/semantic_analyzer/external/util.h
        src/parser/ast/var_reference/array_access_ast_node.cpp
        src/parser/ast/var_reference/array_access_ast_node.h
        src/parser/ast/operator/assignment/cast_ast_node.cpp
        src/parser/ast/operator/assignment/cast_ast_node.h
        src/semantic_analyzer/util/widen_util.cpp
        src/semantic_analyzer/util/widen_util.h
        src/semantic_analyzer/util/poly_util.cpp
        src/semantic_analyzer/util/poly_util.h
        src/lsp/lsp_receiever.cpp
        src/lsp/lsp_receiver.h
        src/parser/ast/var_reference/reference_association.h
)

# sudo dnf install boost-regex boost-devel
find_package(Boost REQUIRED COMPONENTS regex headers)

target_link_libraries(codeshc PRIVATE Boost::boost Boost::regex)

find_package(ZLIB REQUIRED)
target_link_libraries(codeshc PRIVATE ZLIB::ZLIB)

# sudo dnf install libicu-devel
find_package(ICU REQUIRED COMPONENTS i18n uc data)
include_directories(${ICU_INCLUDE_DIRS})

target_link_libraries(codeshc PRIVATE
        ${ICU_LIBRARIES} icui18n
        icuuc
        icudata
)


include(FetchContent)


# utfcpp
# Declare utfcpp content
FetchContent_Declare(
        utfcpp
        GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
        GIT_TAG        origin/master
        GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(utfcpp)

target_include_directories(codeshc PRIVATE
        ${utfcpp_SOURCE_DIR}/source
)

# fmt
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 12.1.0
)

FetchContent_MakeAvailable(fmt)
target_link_libraries(codeshc PRIVATE fmt::fmt)

# nlohmann_json
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_link_libraries(codeshc PRIVATE nlohmann_json::nlohmann_json)


# Support includes starting from relative paths
target_include_directories(codeshc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS codeshc DESTINATION .)
