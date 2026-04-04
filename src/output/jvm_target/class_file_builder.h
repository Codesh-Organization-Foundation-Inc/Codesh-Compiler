#pragma once

#include "output/ir/code_block.h"
#include "output/jvm_target/defs/class_file.h"

#include <unordered_map>

namespace codesh::semantic_analyzer
{
class method_scope_symbol;
class symbol_table;
}
namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
class field_declaration_ast_node;
}
namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::output::jvm_target
{
class constant_pool;
}
namespace codesh::ast
{
namespace type
{
class type_ast_node;
}
namespace method
{
class method_scope_ast_node;
class method_declaration_ast_node;
}
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target
{

constexpr int JAVA_TARGET_VERSION = 65;
constexpr int JAVA_RELEASE_VERSION = 21;

enum class access_flag : uint16_t
{
    ACC_PUBLIC      = 0x0001,
    ACC_PRIVATE     = 0x0002,
    ACC_PROTECTED   = 0x0004,
    ACC_STATIC      = 0x0008,
    ACC_FINAL       = 0x0010,
    ACC_SUPER       = 0x0020,
    ACC_INTERFACE   = 0x0200,
    ACC_ABSTRACT    = 0x0400,
    ACC_SYNTHETIC   = 0x1000,
    ACC_ANNOTATION  = 0x2000,
    ACC_ENUM        = 0x4000,
    ACC_MODULE      = 0x8000
};

class class_file_builder
{
    defs::class_file &class_file;

    const ast::compilation_unit_ast_node &root_node;
    const semantic_analyzer::symbol_table &symbol_table;
    const ast::type_decl::type_declaration_ast_node &type_decl;

    const constant_pool &constant_pool_;

    // Common pools
    const int this_class_cpi;
    const int super_class_cpi;


    void handle_class_type(const ast::type_decl::class_declaration_ast_node &class_decl) const;


    [[nodiscard]] std::unique_ptr<defs::methods_info_entry> create_method_entry(
            const ast::method::method_declaration_ast_node &method_decl) const;
    [[nodiscard]] std::unique_ptr<defs::methods_info_entry> create_raw_method_entry(
            const std::vector<access_flag> &flags,
            const std::string &name,
            const std::string &descriptor) const;

    [[nodiscard]] std::unique_ptr<defs::code_attribute_entry> create_code_attribute(
            const ast::method::method_declaration_ast_node &method_decl) const;

    [[nodiscard]] std::unique_ptr<defs::exceptions_attribute_entry> create_exceptions_attribute(
            const ast::method::method_declaration_ast_node &method_decl) const;

    ir::code_block emit_method_bytecode(defs::code_attribute_entry &code_attr,
                              const ast::method::method_declaration_ast_node &method_decl) const;
    static void emit_code_block_to_attr(const ir::code_block &code_block,
                              defs::code_attribute_entry &code_attr);

    [[nodiscard]] std::unique_ptr<defs::code_attribute_entry> create_code_attr_entry() const;
    static void finalize_code_attr(defs::code_attribute_entry &code_attr);
    [[nodiscard]] static int get_locals_count(const ast::method::method_declaration_ast_node &method_decl);
    [[nodiscard]] std::unique_ptr<defs::local_variable_table_attribute_entry> create_local_variable_table(
        const ast::method::method_declaration_ast_node &method_decl, int code_length_total, int lvt_size) const;

    /**
     * Processes scope markers in the IR to compute bytecode positions for local variables.
     * Must be called before creating LocalVariableTable.
     */
    static void compute_local_variable_bytecode_ranges(const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl, size_t total_code_length);

    /**
     * Maps a scope node to a pair representing the start and end bytecodes
     */
    using scope_to_bytecode_boundaries = std::unordered_map<
        const ast::method::method_scope_ast_node *,
        std::pair<size_t, size_t>
    >;

    [[nodiscard]] static scope_to_bytecode_boundaries create_scope_boundaries(const ir::code_block &method_code,
            const ast::method::method_declaration_ast_node &method_decl, size_t total_code_length);

    static void set_scope_bytecode_boundaries(const ast::method::method_scope_ast_node &scope,
            const scope_to_bytecode_boundaries &scope_boundaries);


    void add_constant_pool_entries() const;
    void add_interfaces() const;
    void add_method(const ast::method::method_declaration_ast_node &method_decl) const;

    /**
     * @returns @c true if a <clinit> was emitted, @c false if there were no static initializers.
     */
    [[nodiscard]] bool emit_static_initializer(
            const ast::type_decl::class_declaration_ast_node &class_decl) const;

    [[nodiscard]] static std::vector<const ast::type_decl::field_declaration_ast_node *>
            collect_static_init_fields(const ast::type_decl::type_declaration_ast_node &type_decl);
    void add_field(const ast::type_decl::field_declaration_ast_node &field_decl) const;
    [[nodiscard]] std::unique_ptr<defs::fields_info_entry> create_field_entry(
            const ast::type_decl::field_declaration_ast_node &field_decl) const;
    void add_source_file() const;

    static void set_access_flags(unsigned char buffer[], const std::vector<access_flag> &flags);

    void collect_local_variables(std::vector<std::unique_ptr<defs::local_variable_table_entry>> &results_out,
            const ast::method::method_declaration_ast_node &method_decl, int code_length_total) const;

public:
    class_file_builder(defs::class_file &class_file_out,
            const ast::compilation_unit_ast_node &root_node,
            const semantic_analyzer::symbol_table &symbol_table,
            const ast::type_decl::type_declaration_ast_node &type_decl);

    void build() const;
};

}
