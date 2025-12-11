#include "class_file_builder.h"

#include "../../defenition/definitions.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/method/method_declaration_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../../util.h"
#include "../ir/code_block.h"
#include "constant_pool.h"

#include "./defs/attribute_info_entry.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <list>
#include <ranges>

codesh::output::jvm_target::class_file_builder::class_file_builder(defs::class_file &class_file_out,
        const ast::compilation_unit_ast_node &root_node,
        const ast::type_decl::type_declaration_ast_node &type_decl) :
    class_file(class_file_out),
    root_node(root_node),
    type_decl(type_decl),
    constant_pool_(type_decl.get_constant_pool()->get()),

    this_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_binary_name())
    )),
    super_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_super_class()->get_binary_name())
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
        throw std::runtime_error("Too many constant pool entries; max amount is 65535");

    util::put_int_bytes(class_file.constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)


    for (const auto &constant_pool_entry : constant_pool_.get_literals())
    {
        class_file.constant_pool.push_back(constant_pool_entry);
    }
}


void codesh::output::jvm_target::class_file_builder::add_method(const ast::method::method_declaration_ast_node &method_decl)
    const
{
    auto method_entry = std::make_unique<defs::methods_info_entry>();

    set_access_flags(method_entry->access_flags, method_decl.get_attributes()->get_access_flags());

    util::put_int_bytes(
        method_entry->name_index, 2,
        constant_pool_.get_utf8_index(method_decl.get_name())
    );
    util::put_int_bytes(
        method_entry->descriptor_index, 2,
    constant_pool_.get_utf8_index(method_decl.generate_descriptor())
    );

    util::put_int_bytes(method_entry->attributes_count, 2, 1);


    // Code
    auto code_attr = std::make_unique<defs::code_attribute_entry>();

    util::put_int_bytes(code_attr->attribute_name_index, 2, constant_pool_.get_utf8_index("Code"));
    util::put_int_bytes(code_attr->max_stack, 2, 1);
    util::put_int_bytes(code_attr->max_locals, 2, 1);


    // Convert the method to IR
    const auto code_block = ir::code_block::build_from_method(
        method_decl,
        root_node.get_symbol_table().value(),
        type_decl
    );

    std::list<unsigned char> bytecode_collector;
    for (const auto &instruction : code_block.get_instructions())
    {
        instruction->emit(bytecode_collector);
    }

    code_attr->code.reserve(bytecode_collector.size());
    code_attr->code.insert(
        std::end(code_attr->code),
        std::begin(bytecode_collector),
        std::end(bytecode_collector)
    );


    if (code_attr->code.size() > 0xFFFFFF)
        throw std::runtime_error("Method code is too long");

    util::put_int_bytes(code_attr->code_length, 4, code_attr->code.size()); // NOLINT(*-narrowing-conversions)


    util::put_int_bytes(code_attr->exception_table_length, 2, 0);

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


    // Local variables
    auto local_variable_table = std::make_unique<defs::local_variable_table_attribute_entry>();
    util::put_int_bytes(
        local_variable_table->attribute_name_index, 2,
        constant_pool_.get_utf8_index("LocalVariableTable")
    );

    std::list<std::unique_ptr<defs::local_variable_table_entry>> lvt_entries;
    add_local_variables(
        lvt_entries,
        method_decl.get_symbol().get_scope(),
        static_cast<int>(code_attr->code.size())
    );

    if (lvt_entries.size() > 0xFFFF)
        throw std::runtime_error("Too many local variables in method " + method_decl.get_name() + "; Max amount is 65535");


    local_variable_table->local_variable_table.reserve(lvt_entries.size());
    for (auto &lvt_entry : lvt_entries)
    {
        local_variable_table->local_variable_table.push_back(std::move(lvt_entry));
    }


    const int lvt_size = static_cast<int>(lvt_entries.size());
    util::put_int_bytes(local_variable_table->local_variable_table_length, 2, lvt_size);

    util::put_int_bytes(
        local_variable_table->attribute_length, 4,
        2 + static_cast<int>(sizeof(defs::local_variable_table_entry)) * lvt_size
    );
    code_attr->attributes.push_back(std::move(local_variable_table));



    util::put_int_bytes(
        code_attr->attribute_length, 4,
        30 + static_cast<int>(code_attr->code.size())
    );
    util::put_int_bytes(code_attr->attribute_count, 2, 1);

    method_entry->attribute_info.push_back(std::move(code_attr));
    class_file.methods_info.push_back(std::move(method_entry));
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

    for (auto flag: flags)
    {
        value |= static_cast<uint16_t>(flag);
    }

    util::put_int_bytes(buffer, 2, value);
}

void codesh::output::jvm_target::class_file_builder::add_local_variables(
        std::list<std::unique_ptr<defs::local_variable_table_entry>> &lvt, semantic_analyzer::method_scope_symbol &scope,
        const int code_length_total) const
{
    for (const auto &[var_name, var] : scope.get_variables())
    {
        auto entry = std::make_unique<defs::local_variable_table_entry>();

        //TODO: Fill per scope info:
        util::put_int_bytes(entry->start_pc, 2, 0);
        util::put_int_bytes(entry->length, 2, code_length_total);

        util::put_int_bytes(
            entry->name_index,
            2,
            constant_pool_.get_utf8_index(
                var_name
            )
        );
        util::put_int_bytes(
            entry->descriptor_index,
            2,
            constant_pool_.get_utf8_index(
                var->get_type()->generate_descriptor()
            )
        );

        util::put_int_bytes(entry->index, 2, static_cast<int>(lvt.size()));

        lvt.push_back(std::move(entry));
    }


    for (const auto &inner_scope : scope.get_inner_scopes())
    {
        add_local_variables(lvt, *inner_scope, code_length_total);
    }
}
