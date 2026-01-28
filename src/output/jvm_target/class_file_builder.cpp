#include "class_file_builder.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/definitions.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "util.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction.h"
#include "constant_pool.h"

#include "output/jvm_target/defs/attribute_info_entry.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include <algorithm>
#include <filesystem>
#include <functional>

#include <list>
#include <ranges>
#include <set>
#include <unordered_map>

codesh::output::jvm_target::class_file_builder::class_file_builder(defs::class_file &class_file_out,
        const ast::compilation_unit_ast_node &root_node,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &type_decl) :
    class_file(class_file_out),
    root_node(root_node),
    symbol_table(symbol_table),
    type_decl(type_decl),
    constant_pool_(type_decl.get_constant_pool()),

    this_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_resolved_name().join())
    )),
    super_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_super_class()->get_resolved_name().join())
    ))
{}

void codesh::output::jvm_target::class_file_builder::build() const
{
    util::put_bytes(class_file.magic, {0xCA, 0xFE, 0xBA, 0xBE});

    util::put_int_bytes(class_file.minor_version, 2, 0);
    util::put_int_bytes(class_file.major_version, 2, JAVA_TARGET_VERSION);

    add_constant_pool_entries();

    //TODO: Set by class attributes
    set_access_flags(class_file.access_flags, {access_flag::ACC_SUPER, access_flag::ACC_PUBLIC});

    util::put_int_bytes(class_file.this_class, 2, this_class_cpi);
    util::put_int_bytes(class_file.super_class, 2, super_class_cpi);

    util::put_int_bytes(class_file.interfaces_count, 2, 0);
    util::put_int_bytes(class_file.fields_count, 2, 0);

    if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(&type_decl))
    {
        handle_class_type(*class_decl);
    }
    else
    {
        // TODO: Handle
        util::put_int_bytes(class_file.methods_count, 2, 0);
    }

    util::put_int_bytes(class_file.attribute_count, 2, 1);

    add_source_file();
}

void codesh::output::jvm_target::class_file_builder::handle_class_type(
    const ast::type_decl::class_declaration_ast_node &class_decl) const
{
    for (const auto &method_decl : class_decl.get_all_methods())
    {
        add_method(*method_decl);
    }

    util::put_int_bytes(
        class_file.methods_count, 2,
        static_cast<int>(class_decl.get_all_methods().size())
    );
}

void codesh::output::jvm_target::class_file_builder::add_constant_pool_entries() const
{
    const size_t constant_pool_size = constant_pool_.size() + 1;

    if (constant_pool_size > 0xFFFF)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::CONSTANT_POOL_TOO_BIG,
            blasphemy::blasphemy_type::OUTPUT, std::nullopt, true);
    }

    util::put_int_bytes(class_file.constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)


    for (const auto &constant_pool_entry : constant_pool_.get_literals())
    {
        class_file.constant_pool.push_back(constant_pool_entry);
    }
}


void codesh::output::jvm_target::class_file_builder::add_method(
        const ast::method::method_declaration_ast_node &method_decl) const
{
    auto method_entry = create_method_entry(method_decl);

    method_entry->attribute_info.push_back(create_code_attribute(method_decl));

    class_file.methods_info.push_back(std::move(method_entry));
}

std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry> codesh::output::jvm_target::class_file_builder::
    create_method_entry(const ast::method::method_declaration_ast_node &method_decl) const
{
    auto method_entry = std::make_unique<defs::methods_info_entry>();

    set_access_flags(method_entry->access_flags, method_decl.get_attributes()->get_access_flags());

    util::put_int_bytes(
        method_entry->name_index, 2,
        constant_pool_.get_utf8_index(method_decl.get_last_name(true))
    );
    util::put_int_bytes(
        method_entry->descriptor_index, 2,
        constant_pool_.get_utf8_index(method_decl.generate_descriptor())
    );

    util::put_int_bytes(method_entry->attributes_count, 2, 1);

    return method_entry;
}

std::unique_ptr<codesh::output::jvm_target::defs::code_attribute_entry> codesh::output::jvm_target::class_file_builder::
        create_code_attribute(const ast::method::method_declaration_ast_node &method_decl) const
{
    auto code_attr = std::make_unique<defs::code_attribute_entry>();

    util::put_int_bytes(code_attr->attribute_name_index, 2, constant_pool_.get_utf8_index("Code"));
    const auto method_code = emit_method_bytecode(*code_attr, method_decl);

    // Compute bytecode positions for local variables before creating LocalVariableTable
    compute_local_variable_bytecode_ranges(method_code, method_decl, code_attr->code.size());

    const auto locals = get_locals_count(method_decl);
    util::put_int_bytes(code_attr->max_locals, 2, locals);

    code_attr->attributes.push_back(create_local_variable_table(
        method_decl,
        static_cast<int>(code_attr->code.size()),
        locals
    ));

    if (method_decl.has_inner_scopes())
    {
        code_attr->attributes.push_back(create_stack_map_table_attribute(method_code));
    }


    size_t attributes_size = 0;
    for (const auto &attribute : code_attr->attributes)
    {
        attributes_size += 6 + util::read_int_bytes(attribute->attribute_length, 4);
    }

    util::put_int_bytes(
        code_attr->attribute_length, 4,
        12
            + static_cast<int>(attributes_size)
            + static_cast<int>(code_attr->code.size())
    );
    util::put_int_bytes(code_attr->attribute_count, 2, static_cast<int>(code_attr->attributes.size()));

    return code_attr;
}

codesh::output::ir::code_block codesh::output::jvm_target::class_file_builder::emit_method_bytecode(
    defs::code_attribute_entry &code_attr, const ast::method::method_declaration_ast_node &method_decl) const
{
    // Convert the method to IR
    ir::code_block code_block;
    method_decl.get_method_scope().emit_ir(code_block, symbol_table, type_decl);

    std::list<ir::instruction_container> bytecode_collector;
    for (const auto &instruction : code_block.get_instructions())
    {
        instruction->emit(bytecode_collector);
    }

    code_attr.code.reserve(bytecode_collector.size());
    int max_stack = 0;
    int current_stack = 0;

    for (const auto &[opcodes, stack_delta] : bytecode_collector)
    {
        for (const auto opcode : opcodes)
        {
            code_attr.code.push_back(opcode);
        }

        // Each opcode may alter the method stack.
        // Count it here:
        current_stack += stack_delta;

        if (current_stack < 0)
            throw std::runtime_error("Stack underflow: Attempted to emit an opcode that pops the stack more than it can");

        max_stack = std::max(max_stack, current_stack);
    }

    util::put_int_bytes(code_attr.max_stack, 2, max_stack);


    if (code_attr.code.size() > 0xFFFFFF)
    {
        blasphemy::blasphemy_collector().add_blasphemy(blasphemy::details::METHOD_TOO_BIG,
            blasphemy::blasphemy_type::OUTPUT, std::nullopt, true);
    }

    util::put_int_bytes(code_attr.code_length, 4, code_attr.code.size()); // NOLINT(*-narrowing-conversions)


    util::put_int_bytes(code_attr.exception_table_length, 2, 0);

    //TODO: Re-add line number table
    //
    // // Line number table
    // auto line_number_table_attr = std::make_unique<codesh::output::jvm_target::defs::line_number_table_attribute_entry>();
    // codesh::util::put_int_bytes(line_number_table_attr->attribute_name_index, 2, 10);
    // codesh::util::put_int_bytes(line_number_table_attr->attribute_length, 4, 6);
    // codesh::util::put_int_bytes(line_number_table_attr->line_number_table_length, 2, 1);
    //
    // auto lnt_entry = std::make_unique<codesh::output::jvm_target::defs::line_number_table_entry>();
    // codesh::util::put_int_bytes(lnt_entry->start_pc, 2, 0);
    // codesh::util::put_int_bytes(lnt_entry->line_number, 2, 1);
    //
    // line_number_table_attr->line_number_table.push_back(std::move(lnt_entry));
    // code_attr->attributes.push_back(std::move(line_number_table_attr));

    return code_block;
}

int codesh::output::jvm_target::class_file_builder::get_locals_count(
        const ast::method::method_declaration_ast_node &method_decl)
{
    // Local variables
    const size_t local_vars_count = method_decl.get_resolved().get_all_local_variables().size();
    if (local_vars_count > 0xFFFF)
    {
        //TODO: Convert to Codesh error
        throw std::runtime_error("Too many local variables in method " + method_decl.get_unresolved_name().join() + "; Max amount is 65535");
    }

    return static_cast<int>(local_vars_count);
}

std::unique_ptr<codesh::output::jvm_target::defs::local_variable_table_attribute_entry> codesh::output::jvm_target::
    class_file_builder::create_local_variable_table(const ast::method::method_declaration_ast_node &method_decl,
        const int code_length_total, const int lvt_size) const
{
    auto local_variable_table = std::make_unique<defs::local_variable_table_attribute_entry>();

    util::put_int_bytes(
        local_variable_table->attribute_name_index, 2,
        constant_pool_.get_utf8_index("LocalVariableTable")
    );

    collect_local_variables(
        local_variable_table->local_variable_table,
        method_decl,
        code_length_total
    );

    util::put_int_bytes(local_variable_table->local_variable_table_length, 2, lvt_size);

    util::put_int_bytes(
        local_variable_table->attribute_length, 4,
        2 + static_cast<int>(sizeof(defs::local_variable_table_entry)) * lvt_size
    );

    return local_variable_table;
}

std::unique_ptr<codesh::output::jvm_target::defs::stack_map_table_attribute_entry> codesh::output::jvm_target::
        class_file_builder::create_stack_map_table_attribute(const ir::code_block &method_code) const
{
    auto smt_attr = std::make_unique<defs::stack_map_table_attribute_entry>();

    util::put_int_bytes(smt_attr->attribute_name_index, 2, constant_pool_.get_utf8_index("StackMapTable"));
    add_stack_map_frames(*smt_attr, method_code);

    return smt_attr;
}

std::set<size_t> codesh::output::jvm_target::class_file_builder::collect_jump_targets(
    const ir::code_block &method_code)
{
    std::set<size_t> jump_targets;
    jump_targets.insert(0);

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

void codesh::output::jvm_target::class_file_builder::add_stack_map_frames(
        defs::stack_map_table_attribute_entry &smt_attr, const ir::code_block &method_code)
{
    const auto frame_targets = collect_jump_targets(method_code);

    size_t smt_attr_size = 2;

    int prev_pos = -1;
    for (const size_t target : frame_targets)
    {
        const int current_pos = static_cast<int>(target);
        const int offset_delta = current_pos - prev_pos - 1;

        std::unique_ptr<defs::stack_map_frame> frame;
        //TODO: Figure out when it's not same_frame
        if (offset_delta <= 63)
        {
            frame = std::make_unique<defs::same_frame>(static_cast<unsigned char>(offset_delta));
            smt_attr_size += 1;
        }
        else
        {
            if (offset_delta > 0xFFFF)
            {
                //TODO: Make Codesh error
                throw std::runtime_error("Inner scope too big; Max is 65535");
            }

            auto temp_frame = std::make_unique<defs::same_frame_extended>();
            util::put_int_bytes(temp_frame->offset_delta, 2, offset_delta);

            frame = std::move(temp_frame);
            smt_attr_size += 3;
        }

        smt_attr.entries.push_back(std::move(frame));
        prev_pos = current_pos;
    }

    util::put_int_bytes(smt_attr.number_of_entries, 2, static_cast<int>(smt_attr.entries.size()));
    util::put_int_bytes(smt_attr.attribute_length, 4, static_cast<int>(smt_attr_size));
}

void codesh::output::jvm_target::class_file_builder::add_source_file() const
{
    auto source_file_entry = std::make_unique<defs::source_file_attribute_entry>();
    util::put_int_bytes(
        source_file_entry->attribute_name_index, 2,
    constant_pool_.get_utf8_index("SourceFile")
    );

    util::put_int_bytes(source_file_entry->attribute_length, 4, 2);
    util::put_int_bytes(source_file_entry->sourcefile_index, 2,
        constant_pool_.get_utf8_index(root_node.get_source_stem() + definition::SOURCE_FILE_EXTENSION)
    );

    class_file.attribute_info.push_back(std::move(source_file_entry));
}

void codesh::output::jvm_target::class_file_builder::set_access_flags(
        unsigned char buffer[], const std::vector<access_flag> &flags)
{
    //TODO: Change default values
    uint16_t value = 0;

    for (const auto flag : flags)
    {
        value |= static_cast<uint16_t>(flag);
    }

    util::put_int_bytes(buffer, 2, value);
}

void codesh::output::jvm_target::class_file_builder::collect_local_variables(
        std::vector<std::unique_ptr<defs::local_variable_table_entry>> &results_out,
        const ast::method::method_declaration_ast_node &method_decl,
        const int code_length_total) const
{
    for (const auto &[name, var] : method_decl.get_resolved().get_all_local_variables())
    {
        auto entry = std::make_unique<defs::local_variable_table_entry>();

        // Use bytecode positions computed by compute_local_variable_bytecode_ranges
        const auto *producing_node = var.get().get_producing_node();
        const size_t start_pc = producing_node ? producing_node->get_bytecode_start_pc() : 0;
        const size_t length = producing_node ? producing_node->get_bytecode_length() : static_cast<size_t>(code_length_total);

        util::put_int_bytes(entry->start_pc, 2, static_cast<int>(start_pc));
        util::put_int_bytes(entry->length, 2, static_cast<int>(length));

        util::put_int_bytes(
            entry->name_index,
            2,
            constant_pool_.get_utf8_index(
                name
            )
        );
        util::put_int_bytes(
            entry->descriptor_index,
            2,
            constant_pool_.get_utf8_index(
                var.get().get_type()->generate_descriptor()
            )
        );


        const size_t var_index = var.get().get_index();
        if (var_index > 0xFFFF)
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    "יותר מידיי משתנים מקומיים במעשה {}",
                    method_decl.get_resolved_name().holy_join()
                ),
                blasphemy::blasphemy_type::OUTPUT,
                std::nullopt,
                true
            );
        }

        util::put_int_bytes(entry->index, 2, static_cast<int>(var_index));

        results_out.push_back(std::move(entry));
    }
}

void codesh::output::jvm_target::class_file_builder::compute_local_variable_bytecode_ranges(
        const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl,
        const size_t total_code_length)
{
    const auto scope_boundaries = create_scope_boundaries(method_code, method_decl, total_code_length);

    // Second pass: set bytecode ranges for all local variables based on their containing scope
    // Variables in root method scope
    for (const auto &var : method_decl.get_method_scope().get_local_variables())
    {
        var->set_bytecode_start_pc(0);
        var->set_bytecode_length(total_code_length);
    }

    // Variables in inner scopes - need to recursively process
    std::function<void(const ast::method::method_scope_ast_node &)> process_scope;
    process_scope = [&](const ast::method::method_scope_ast_node &scope)
    {
        auto it = scope_boundaries.find(&scope);
        if (it == scope_boundaries.end())
            return;

        const auto [scope_start, scope_end] = it->second;

        for (const auto &var : scope.get_local_variables())
        {
            var->set_bytecode_start_pc(scope_start);
            var->set_bytecode_length(scope_end - scope_start);
        }

        // Process nested scopes
        for (const auto &inner_scope : scope.get_method_scopes())
        {
            process_scope(*inner_scope);
        }
    };

    // Process all inner scopes
    for (const auto &inner_scope : method_decl.get_method_scope().get_method_scopes())
    {
        process_scope(*inner_scope);
    }
}

codesh::output::jvm_target::class_file_builder::scope_to_bytecode_boundaries codesh::output::jvm_target::
    class_file_builder::create_scope_boundaries(const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl,
        const size_t total_code_length)
{
    scope_to_bytecode_boundaries result;

    // The root method scope spans the entire method
    result[&method_decl.get_method_scope()] = {0, total_code_length};

    size_t current_bytecode_pos = 0;
    for (const auto &instr : method_code.get_instructions())
    {
        if (auto *begin_marker = dynamic_cast<ir::scope_begin_marker *>(instr.get()))
        {
            begin_marker->set_bytecode_position(current_bytecode_pos);
            result[&begin_marker->get_scope()].first = current_bytecode_pos;
        }
        else if (auto *end_marker = dynamic_cast<ir::scope_end_marker *>(instr.get()))
        {
            end_marker->set_bytecode_position(current_bytecode_pos);
            result[&end_marker->get_scope()].second = current_bytecode_pos;
        }

        current_bytecode_pos += instr->size();
    }

    return result;
}
