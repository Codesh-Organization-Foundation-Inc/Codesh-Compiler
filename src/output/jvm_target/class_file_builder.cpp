#include "class_file_builder.h"

#include "../../defenition/definitions.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/method/method_declaration_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../../util.h"
#include "constant_pool.h"

#include "./defs/attribute_info_entry.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <list>
#include <ranges>


codesh::output::jvm_target::class_file_builder::class_file_builder(const ast::compilation_unit_ast_node &root_node,
        const ast::type_decl::type_declaration_ast_node &type_decl) :
    class_file(std::make_unique<defs::class_file>()),
    root_node(root_node),
    type_decl(type_decl),
    constant_pool_(type_decl.get_constant_pool()->get()),

    this_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_binary_name())
    )),
    super_class_cpi(constant_pool_.get_class_index(
        constant_pool_.get_utf8_index(type_decl.get_binary_name())
    ))
{}

std::unique_ptr<codesh::output::jvm_target::defs::class_file> codesh::output::jvm_target::class_file_builder::build()
{
    util::put_bytes(class_file->magic, {0xCA, 0xFE, 0xBA, 0xBE});

    util::put_int_bytes(class_file->minor_version, 2, 0);
    util::put_int_bytes(class_file->major_version, 2, JAVA_TARGET_VERSION);

    add_constant_pool_entries();

    //TODO: Set by class attributes
    set_access_flags(class_file->access_flags, {access_flag::ACC_SUPER, access_flag::ACC_PUBLIC});

    util::put_int_bytes(class_file->this_class, 2, this_class_cpi);
    util::put_int_bytes(class_file->super_class, 2, super_class_cpi);

    util::put_int_bytes(class_file->interfaces_count, 2, 0);
    util::put_int_bytes(class_file->fields_count, 2, 0);

    if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(&type_decl))
    {
        handle_class_type(*class_decl);
    }
    else
    {
        // TODO: Handle
        util::put_int_bytes(class_file->methods_count, 2, 0);
    }

    util::put_int_bytes(class_file->attribute_count, 2, 1);

    add_source_file();

    return std::move(class_file);
}

void codesh::output::jvm_target::class_file_builder::handle_class_type(
    const ast::type_decl::class_declaration_ast_node &class_decl) const
{
    for (const auto &method_decl : class_decl.get_methods())
    {
        add_method(*method_decl);
    }

    util::put_int_bytes(
        class_file->methods_count, 2,
        static_cast<int>(class_decl.get_methods().size())
    );
}

void codesh::output::jvm_target::class_file_builder::add_constant_pool_entries() const
{
    const size_t constant_pool_size = constant_pool_.size() + 1;

    if (constant_pool_size > 0xFFFF)
        throw std::runtime_error("Too many constant pool entries; max amount is 65535");

    util::put_int_bytes(class_file->constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)


    for (const auto &constant_pool_entry : constant_pool_.get_literals())
    {
        class_file->constant_pool.push_back(constant_pool_entry);
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

    // Actual bytecode
    //TODO: This should already be integrated in constructor's IR.
    if (dynamic_cast<const ast::method::constructor_declaration_ast_node *>(&method_decl))
    {
        code_attr->code.push_back(0x2A);
        {
            const unsigned char super_call[] = {
                0xB7, 0x00, static_cast<unsigned char>(constant_pool_.get_methodref_index(
                    super_class_cpi,
                    constant_pool_.get_name_and_type_index(
                        constant_pool_.get_utf8_index("<init>"),
                        constant_pool_.get_utf8_index("()V")
                    )
                ))
            };

            for (const unsigned char bytecode : super_call)
            {
                code_attr->code.push_back(bytecode);
            }
        }
    }

    code_attr->code.push_back(0xB1);


    if (code_attr->code.size() > 0xFFFFFF)
        throw std::runtime_error("Method code is too long");

    util::put_int_bytes(code_attr->code_length, 4, code_attr->code.size()); // NOLINT(*-narrowing-conversions)


    util::put_int_bytes(code_attr->exception_table_length, 2, 0);

    //TODO: Re-add line number table

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

    // Add parameters
    int index = 0;
    for (const auto &param_node : method_decl.get_parameters())
    {
        auto lvt_entry = std::make_unique<defs::local_variable_table_entry>();
        util::put_int_bytes(lvt_entry->start_pc, 2, 0);
        util::put_int_bytes(lvt_entry->length, 2, static_cast<int>(code_attr->code.size()));

        util::put_int_bytes(
            lvt_entry->name_index, 2,
            constant_pool_.get_utf8_index(param_node->get_name())
        );
        util::put_int_bytes(
            lvt_entry->descriptor_index, 2,
            constant_pool_.get_utf8_index(param_node->get_type()->generate_descriptor())
        );

        util::put_int_bytes(lvt_entry->index, 2, index++);
        local_variable_table->local_variable_table.push_back(std::move(lvt_entry));
    }

    //TODO: Account for local variables too
    const int local_vars_count = static_cast<int>(method_decl.get_parameters().size());

    util::put_int_bytes(local_variable_table->local_variable_table_length, 2, local_vars_count);

    util::put_int_bytes(
        local_variable_table->attribute_length, 4,
        2 + static_cast<int>(sizeof(defs::local_variable_table_entry)) * local_vars_count
    );
    code_attr->attributes.push_back(std::move(local_variable_table));


    util::put_int_bytes(
        code_attr->attribute_length, 4,
        30 + static_cast<int>(code_attr->code.size())
    );
    util::put_int_bytes(code_attr->attribute_count, 2, 1);

    method_entry->attribute_info.push_back(std::move(code_attr));
    class_file->methods_info.push_back(std::move(method_entry));
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

    class_file->attribute_info.push_back(std::move(source_file_entry));
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
