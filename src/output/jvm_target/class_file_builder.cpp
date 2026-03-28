#include "class_file_builder.h"
#include "stack_map_table_builder.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "constant_pool.h"
#include "defenition/definitions.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/scope_marker.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/ast/type_declaration/field_declaration_ast_node.h"
#include "output/jvm_target/defs/fields_info_entry.h"
#include "util.h"

#include "output/jvm_target/defs/attribute_info_entry.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include <algorithm>
#include <filesystem>
#include <functional>

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

    add_interfaces();

    if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(&type_decl))
    {
        handle_class_type(*class_decl);
    }
    else
    {
        // TODO: Handle
        util::put_int_bytes(class_file.fields_count, 2, 0);
        util::put_int_bytes(class_file.methods_count, 2, 0);
    }

    util::put_int_bytes(class_file.attribute_count, 2, 1);

    add_source_file();
}

void codesh::output::jvm_target::class_file_builder::handle_class_type(
    const ast::type_decl::class_declaration_ast_node &class_decl) const
{
    for (const auto &field_decl : class_decl.get_fields())
    {
        add_field(*field_decl);
    }

    util::put_int_bytes(
        class_file.fields_count, 2,
        static_cast<int>(class_decl.get_fields().size())
    );

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
            blasphemy::blasphemy_type::OUTPUT, lexer::NO_CODE_POS, true);
    }

    util::put_int_bytes(class_file.constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)


    for (const auto &constant_pool_entry : constant_pool_.get_literals())
    {
        class_file.constant_pool.push_back(constant_pool_entry);
    }
}


void codesh::output::jvm_target::class_file_builder::add_field(
        const ast::type_decl::field_declaration_ast_node &field_decl) const
{
    class_file.fields_info.push_back(create_field_entry(field_decl));
}

std::unique_ptr<codesh::output::jvm_target::defs::fields_info_entry> codesh::output::jvm_target::class_file_builder::
    create_field_entry(const ast::type_decl::field_declaration_ast_node &field_decl) const
{
    auto entry = std::make_unique<defs::fields_info_entry>();

    set_access_flags(entry->access_flags, field_decl.get_attributes()->get_access_flags());
    util::put_int_bytes(
        entry->name_index,
        2,
        constant_pool_.get_utf8_index(field_decl.get_name())
    );
    util::put_int_bytes(
        entry->descriptor_index,
        2,
        constant_pool_.get_utf8_index(field_decl.generate_descriptor(true))
    );
    util::put_int_bytes(entry->attributes_count, 2, 0);

    return entry;
}

void codesh::output::jvm_target::class_file_builder::add_method(
        const ast::method::method_declaration_ast_node &method_decl) const
{
    auto method_entry = create_method_entry(method_decl);

    method_entry->attribute_info.push_back(create_code_attribute(method_decl));

    if (!method_decl.get_sins_thrown().empty())
    {
        method_entry->attribute_info.push_back(create_exceptions_attribute(method_decl));
    }

    util::put_int_bytes(method_entry->attributes_count, 2, static_cast<int>(method_entry->attribute_info.size()));

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
        static_cast<int>(method_decl.get_resolved().get_all_local_variables().name_to_var.size())
    ));

    if (method_decl.has_inner_scopes())
    {
        code_attr->attributes.push_back(
            stack_map_table_builder(constant_pool_).build(method_code, method_decl)
        );
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

std::unique_ptr<codesh::output::jvm_target::defs::exceptions_attribute_entry>
    codesh::output::jvm_target::class_file_builder::create_exceptions_attribute(
        const ast::method::method_declaration_ast_node &method_decl) const
{
    auto result = std::make_unique<defs::exceptions_attribute_entry>();
    util::put_int_bytes(result->attribute_name_index, 2, constant_pool_.get_utf8_index("Exceptions"));

    const auto &exceptions = method_decl.get_sins_thrown();
    util::put_int_bytes(result->number_of_exceptions, 2, static_cast<int>(exceptions.size()));

    for (const auto &exception_type : exceptions)
    {
        const std::string class_name = exception_type->get_resolved_name().join();
        const int utf8_idx = constant_pool_.get_utf8_index(class_name);
        const int class_idx = constant_pool_.get_class_index(utf8_idx);

        std::array<unsigned char, 2> entry{};
        util::put_int_bytes(entry.data(), 2, class_idx);
        result->exception_index_table.push_back(entry);
    }

    // attribute_length = 2 (number_of_exceptions) + 2 * count
    util::put_int_bytes(result->attribute_length, 4, 2 + 2 * static_cast<int>(exceptions.size()));

    return result;
}

codesh::output::ir::code_block codesh::output::jvm_target::class_file_builder::emit_method_bytecode(
    defs::code_attribute_entry &code_attr, const ast::method::method_declaration_ast_node &method_decl) const
{
    // Convert the method to IR
    ir::code_block code_block;
    method_decl.get_method_scope().emit_ir(code_block, symbol_table, type_decl);

    std::vector<ir::instruction_container> bytecode_collector;
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
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::METHOD_TOO_BIG,
            blasphemy::blasphemy_type::OUTPUT, lexer::NO_CODE_POS, true
        );
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
    // Local variable slots
    const size_t local_vars_count = method_decl.get_resolved().get_all_local_variables().slots_used;
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

void codesh::output::jvm_target::class_file_builder::add_interfaces() const
{
    const auto &interfaces = type_decl.get_interfaces();
    util::put_int_bytes(class_file.interfaces_count, 2, static_cast<int>(interfaces.size()));

    for (const auto &interface : interfaces)
    {
        std::array<unsigned char, 2> entry{};

        const int class_idx = constant_pool_.get_class_index(
            constant_pool_.get_utf8_index(interface->get_resolved_name().join())
        );
        util::put_int_bytes(entry.data(), 2, class_idx);

        class_file.interfaces_info.push_back(entry);
    }
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
    for (const auto &[name, var] : method_decl.get_resolved().get_all_local_variables().name_to_var)
    {
        auto entry = std::make_unique<defs::local_variable_table_entry>();

        const auto *producing_node = var.get().get_producing_node();

        // Use the positions as computed earlier by compute_local_variable_bytecode_ranges
        const size_t start_pc = producing_node != nullptr
            ? producing_node->get_bytecode_start_pc()
            : 0;
        const size_t length = producing_node != nullptr
            ? producing_node->get_bytecode_length()
            : static_cast<size_t>(code_length_total);

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


        const size_t var_index = var.get().get_jvm_index();
        if (var_index > 0xFFFF)
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    blasphemy::details::TOO_MANY_LOCAL_VARIABLES,
                    method_decl.get_resolved_name().holy_join()
                ),
                blasphemy::blasphemy_type::OUTPUT,
                lexer::NO_CODE_POS,
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
    set_scope_bytecode_boundaries(method_decl.get_method_scope(), scope_boundaries);
}

void codesh::output::jvm_target::class_file_builder::set_scope_bytecode_boundaries(
        const ast::method::method_scope_ast_node &scope,
        const scope_to_bytecode_boundaries &scope_boundaries)
{
    const auto it = scope_boundaries.find(&scope);
    if (it == scope_boundaries.end())
        return;

    const auto [scope_start, scope_end] = it->second;

    for (const auto &var : scope.get_local_variables())
    {
        var->set_bytecode_start_pc(scope_start);
        var->set_bytecode_length(scope_end - scope_start);
    }

    for (const auto &inner_scope : scope.get_method_scopes())
    {
        set_scope_bytecode_boundaries(*inner_scope, scope_boundaries);
    }
}

codesh::output::jvm_target::class_file_builder::scope_to_bytecode_boundaries codesh::output::jvm_target::
    class_file_builder::create_scope_boundaries(const ir::code_block &method_code,
        const ast::method::method_declaration_ast_node &method_decl,
        const size_t total_code_length)
{
    scope_to_bytecode_boundaries result;

    // The root scope spans the entire method
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
