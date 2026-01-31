#include "stack_map_table_builder.h"

#include "constant_pool.h"
#include "util.h"
#include "output/ir/instruction.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include <ranges>

codesh::output::jvm_target::stack_map_table_builder::stack_map_table_builder(const constant_pool &constant_pool)
    : constant_pool_(constant_pool)
{}

std::unique_ptr<codesh::output::jvm_target::defs::stack_map_table_attribute_entry>
    codesh::output::jvm_target::stack_map_table_builder::build(
        const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl) const
{
    auto smt_attr = std::make_unique<defs::stack_map_table_attribute_entry>();

    util::put_int_bytes(smt_attr->attribute_name_index, 2, constant_pool_.get_utf8_index("StackMapTable"));
    add_stack_map_frames(*smt_attr, method_code, method_decl);

    return smt_attr;
}

void codesh::output::jvm_target::stack_map_table_builder::add_stack_map_frames(
        defs::stack_map_table_attribute_entry &smt_attr, const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl) const
{
    const auto frame_targets = collect_jump_targets(method_code);

    auto [entries, total_byte_size] = build_stack_map_entries(
        frame_targets,
        method_decl
    );

    for (auto &entry : entries)
    {
        smt_attr.entries.push_back(std::move(entry));
    }

    const size_t smt_attr_length = 2 + total_byte_size; // 2 bytes for number_of_entries

    util::put_int_bytes(smt_attr.number_of_entries, 2, static_cast<int>(smt_attr.entries.size()));
    util::put_int_bytes(smt_attr.attribute_length, 4, static_cast<int>(smt_attr_length));
}

std::set<size_t> codesh::output::jvm_target::stack_map_table_builder::collect_jump_targets(
    const ir::code_block &method_code)
{
    std::set<size_t> jump_targets;

    size_t curr_byte_pos = 0;
    for (const auto &method_instruction : method_code.get_instructions())
    {
        if (const auto jump_instr = dynamic_cast<const ir::goto_instruction *>(method_instruction.get()))
        {
            const int offset = jump_instr->get_jump_offset() + static_cast<int>(method_instruction->size());
            const int target = static_cast<int>(curr_byte_pos) + offset;

            jump_targets.insert(static_cast<size_t>(target));
        }

        curr_byte_pos += method_instruction->size();
    }

    return jump_targets;
}

codesh::output::jvm_target::stack_map_builder_result codesh::output::jvm_target::stack_map_table_builder::build_stack_map_entries(
        const std::set<size_t> &frame_targets,
        const ast::method::method_declaration_ast_node &method_decl) const
{
    std::vector<std::unique_ptr<defs::stack_map_frame>> entries;
    size_t total_byte_size = 0;

    auto prev_locals = build_local_verifications_at(0, method_decl);

    int prev_pos = -1;
    for (const size_t target : frame_targets)
    {
        const int current_pos = static_cast<int>(target);
        const int offset_delta = current_pos - prev_pos - 1;

        if (offset_delta > 0xFFFF)
        {
            throw std::runtime_error("Inner scope too big; Max is 65535");
        }

        auto current_locals = build_local_verifications_at(target, method_decl);

        auto [frame, byte_size] = build_stack_frame(
            prev_locals,
            current_locals,
            offset_delta
        );
        entries.push_back(std::move(frame));
        total_byte_size += byte_size;

        prev_locals = build_local_verifications_at(target, method_decl);
        prev_pos = current_pos;
    }

    return {std::move(entries), total_byte_size};
}

std::vector<std::unique_ptr<codesh::output::jvm_target::defs::verification_type_info>>
    codesh::output::jvm_target::stack_map_table_builder::build_local_verifications_at(
        const size_t offset, const ast::method::method_declaration_ast_node &method_decl) const
{
    const size_t slots_used = method_decl.get_resolved().get_all_local_variables().slots_used;

    std::vector<std::unique_ptr<defs::verification_type_info>> locals;
    locals.reserve(slots_used);
    for (size_t i = 0; i < slots_used; ++i)
    {
        locals.push_back(std::make_unique<defs::top_variable_info>());
    }

    for (const auto &var : method_decl.get_resolved().get_all_local_variables().name_to_var | std::views::values)
    {
        const auto *producing_node = var.get().get_producing_node();

        // Find all the active variables and add them to the results vector.
        // An active variable is a variable that is within the current scope.
        bool is_active;
        if (producing_node == nullptr)
        {
            // Parameters and 'this' have no producing node: always active.
            is_active = true;
        }
        else
        {
            const size_t start_pc = producing_node->get_bytecode_start_pc();
            const size_t length = producing_node->get_bytecode_length();
            is_active = offset >= start_pc && offset < start_pc + length;
        }

        if (is_active)
        {
            const size_t idx = var.get().get_jvm_index();
            locals[idx] = parse_verification_type(*var.get().get_type());
        }
    }

    // Trim trailing top entries
    while (!locals.empty() && locals.back()->get_tag() == 0)
    {
        locals.pop_back();
    }

    return locals;
}

std::unique_ptr<codesh::output::jvm_target::defs::verification_type_info> codesh::output::jvm_target::
    stack_map_table_builder::parse_verification_type(
        const ast::type::type_ast_node &type_node) const
{
    if (const auto primitive_type = dynamic_cast<const ast::type::primitive_type_ast_node *>(&type_node))
    {
        switch (primitive_type->get_type())
        {
        case definition::primitive_type::INTEGER:
        case definition::primitive_type::BOOLEAN:
        case definition::primitive_type::BYTE:
        case definition::primitive_type::CHAR:
        case definition::primitive_type::SHORT:
            return std::make_unique<defs::integer_variable_info>();

        case definition::primitive_type::FLOAT:
            return std::make_unique<defs::float_variable_info>();

        case definition::primitive_type::LONG:
            return std::make_unique<defs::long_variable_info>();

        case definition::primitive_type::DOUBLE:
            return std::make_unique<defs::double_variable_info>();

        case definition::primitive_type::REFERENCE:
        {
            auto obj_var_info = std::make_unique<defs::object_variable_info>();

            const int class_cpi = constant_pool_.get_class_index(
                constant_pool_.get_utf8_index(
                    primitive_type->generate_descriptor()
                )
            );
            util::put_int_bytes(obj_var_info->cpool_index, 2, class_cpi);

            return obj_var_info;
        }

        default:
            return std::make_unique<defs::top_variable_info>();
        }
    }

    if (const auto custom_type = dynamic_cast<const ast::type::custom_type_ast_node *>(&type_node))
    {
        auto obj_var_info = std::make_unique<defs::object_variable_info>();

        const int class_cpi = constant_pool_.get_class_index(
            constant_pool_.get_utf8_index(
                custom_type->get_array_dimensions() == 0
                    ? custom_type->get_resolved_name().join()
                    : custom_type->generate_descriptor()
            )
        );
        util::put_int_bytes(obj_var_info->cpool_index, 2, class_cpi);

        return obj_var_info;
    }

    throw std::invalid_argument("Invalid type provided");
}

static constexpr size_t MAX_STACK_FRAME_DELTA = 3;
static constexpr size_t MAX_SAME_FRAME_OFFSET = 63;

codesh::output::jvm_target::frame_result codesh::output::jvm_target::stack_map_table_builder::
    build_stack_frame(const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
        std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals,
        const int offset_delta)
{
    const size_t prev_size = prev_locals.size();
    const size_t curr_size = current_locals.size();

    if (are_locals_identical(prev_locals, current_locals))
    {
        if (offset_delta <= MAX_SAME_FRAME_OFFSET)
        {
            return {
                std::make_unique<defs::same_frame>(static_cast<unsigned char>(offset_delta)),
                1
            };
        }

        auto frame = std::make_unique<defs::same_frame_extended>();
        util::put_int_bytes(frame->offset_delta, 2, offset_delta);

        return {std::move(frame), 3};
    }

    //NOTE: If the delta is too complex, resorts to building a full frame.

    if (curr_size > prev_size && curr_size <= prev_size + MAX_STACK_FRAME_DELTA)
    {
        if (auto append_result = try_build_append_frame(offset_delta, prev_size, prev_locals, current_locals))
        {
            return std::move(append_result.value());
        }

        return build_full_frame(offset_delta, current_locals);
    }

    if (curr_size < prev_size && prev_size <= curr_size + MAX_STACK_FRAME_DELTA)
    {
        if (auto chop_result = try_build_chop_frame(offset_delta, prev_size, curr_size, prev_locals, current_locals))
        {
            return std::move(chop_result.value());
        }

        return build_full_frame(offset_delta, current_locals);
    }

    return build_full_frame(offset_delta, current_locals);
}

bool codesh::output::jvm_target::stack_map_table_builder::are_locals_identical(
        const std::vector<std::unique_ptr<defs::verification_type_info>> &locals_1,
        const std::vector<std::unique_ptr<defs::verification_type_info>> &locals_2)
{
    if (locals_1.size() != locals_2.size())
        return false;

    for (size_t i = 0; i < locals_1.size(); ++i)
    {
        if (locals_1.at(i)->get_tag() != locals_2.at(i)->get_tag())
            return false;
    }

    return true;
}

std::optional<codesh::output::jvm_target::frame_result>
    codesh::output::jvm_target::stack_map_table_builder::try_build_append_frame(
        const int offset_delta, const size_t prev_size,
        const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
        std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals)
{
    const size_t curr_size = current_locals.size();

    for (size_t i = 0; i < prev_size; ++i)
    {
        if (prev_locals[i]->get_tag() != current_locals[i]->get_tag())
            return std::nullopt;
    }

    const auto k = static_cast<unsigned char>(curr_size - prev_size);
    auto append = std::make_unique<defs::append_frame>(k);
    util::put_int_bytes(append->offset_delta, 2, offset_delta);

    size_t frame_size = 3; // 1 byte frame_type + 2 bytes offset_delta
    for (size_t i = prev_size; i < curr_size; ++i)
    {
        frame_size += verification_type_byte_size(*current_locals[i]);
        append->locals.push_back(std::move(current_locals[i]));
    }

    return frame_result{std::move(append), frame_size};
}

std::optional<codesh::output::jvm_target::frame_result>codesh::output::jvm_target::stack_map_table_builder::
    try_build_chop_frame(const int offset_delta, const size_t prev_size, const size_t curr_size,
        const std::vector<std::unique_ptr<defs::verification_type_info>> &prev_locals,
        const std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals)
{
    for (size_t i = 0; i < curr_size; ++i)
    {
        if (prev_locals[i]->get_tag() != current_locals[i]->get_tag())
            return std::nullopt;
    }

    const auto k = static_cast<unsigned char>(prev_size - curr_size);
    auto chop = std::make_unique<defs::chop_frame>(k);
    util::put_int_bytes(chop->offset_delta, 2, offset_delta);

    return frame_result{std::move(chop), 3}; // 1 byte frame_type + 2 bytes offset_delta
}

codesh::output::jvm_target::frame_result codesh::output::jvm_target::stack_map_table_builder::build_full_frame(
        const int offset_delta, std::vector<std::unique_ptr<defs::verification_type_info>> &current_locals)
{
    const size_t curr_size = current_locals.size();

    auto full = std::make_unique<defs::full_frame>();
    util::put_int_bytes(full->offset_delta, 2, offset_delta);
    util::put_int_bytes(full->number_of_locals, 2, static_cast<int>(curr_size));
    util::put_int_bytes(full->number_of_stack_items, 2, 0);

    size_t frame_size = 7; // 1 + 2 + 2 + 2 (frame_type + offset_delta + num_locals + num_stack)
    for (size_t i = 0; i < curr_size; ++i)
    {
        frame_size += verification_type_byte_size(*current_locals[i]);
        full->locals.push_back(std::move(current_locals[i]));
    }

    return frame_result{std::move(full), frame_size};
}

size_t codesh::output::jvm_target::stack_map_table_builder::verification_type_byte_size(
    const defs::verification_type_info &info)
{
    const unsigned char tag = info.get_tag();

    // object_variable_info (tag 7) and uninitialized_variable_info (tag 8) have 2 extra bytes
    if (tag == 7 || tag == 8)
        return 3;

    return 1;
}
