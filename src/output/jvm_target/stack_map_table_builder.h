#pragma once

#include "output/ir/code_block.h"
#include "output/jvm_target/defs/class_file.h"

#include <optional>
#include <set>

namespace codesh::ast::type
{
class type_ast_node;
}
namespace codesh::ast::method
{
class method_declaration_ast_node;
}


namespace codesh::output::jvm_target
{
class constant_pool;

struct frame_result
{
    std::unique_ptr<defs::stack_map_frame> frame;
    size_t byte_size;
};
struct stack_map_builder_result
{
    std::vector<std::unique_ptr<defs::stack_map_frame>> entries;
    size_t total_byte_size;
};


class stack_map_table_builder
{
    const constant_pool &constant_pool_;

    void add_stack_map_frames(defs::stack_map_table_attribute_entry &smt_attr,
            const ir::code_block &method_code,
            const ast::method::method_declaration_ast_node &method_decl) const;

    [[nodiscard]] static std::set<size_t> collect_jump_targets(const ir::code_block &method_code);

    [[nodiscard]] stack_map_builder_result build_stack_map_entries(
            const std::set<size_t> &frame_targets,
            const ast::method::method_declaration_ast_node &method_decl) const;

    [[nodiscard]] std::vector<std::unique_ptr<defs::verification_type_info>> build_local_verifications_at(
            size_t offset, const ast::method::method_declaration_ast_node &method_decl) const;

    [[nodiscard]] std::vector<std::unique_ptr<defs::verification_type_info>> build_initial_frame(
            const ast::method::method_declaration_ast_node &method_decl) const;

    [[nodiscard]] std::unique_ptr<defs::verification_type_info> parse_verification_type(
            const ast::type::type_ast_node &type_node) const;

    [[nodiscard]] std::unique_ptr<defs::object_variable_info> build_object_var_info(
            const std::string &descriptor) const;

    [[nodiscard]] static frame_result build_stack_frame(
            const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
            std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals,
            int offset_delta);

    [[nodiscard]] static bool are_locals_identical(
            const std::vector<std::unique_ptr<defs::verification_type_info>> &locals_1,
            const std::vector<std::unique_ptr<defs::verification_type_info>> &locals_2);

    [[nodiscard]] static std::optional<frame_result> try_build_append_frame(
            int offset_delta, size_t prev_size,
            const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
            std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals);

    [[nodiscard]] static std::optional<frame_result> try_build_chop_frame(
            int offset_delta, size_t prev_size, size_t curr_size,
            const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
            const std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals);

    [[nodiscard]] static frame_result build_full_frame(
            int offset_delta,
            std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals);

    [[nodiscard]] static size_t verification_type_byte_size(const defs::verification_type_info &info);

public:
    explicit stack_map_table_builder(const constant_pool &constant_pool);

    [[nodiscard]] std::unique_ptr<defs::stack_map_table_attribute_entry> build(
            const ir::code_block &method_code, const ast::method::method_declaration_ast_node &method_decl) const;
};

}
