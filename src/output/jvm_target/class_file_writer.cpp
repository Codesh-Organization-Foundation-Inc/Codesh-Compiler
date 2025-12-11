#include "class_file_writer.h"

#include <filesystem>
#include <fstream>

#include <ranges>

#include <vector>

#include "defs/methods_info_entry.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../blasphemies/blasphemy_collector.h"

#include "./defs/attribute_info_entry.h"
#include "defs/class_file.h"

static void write_bytes(std::ofstream &out, const unsigned char *data, std::streamsize length);
static void write_methods(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods);
static void write_attributes(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes);
static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file);


void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file,
    const ast::compilation_unit_ast_node &root_node,
    const ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &destination)
{
    std::ofstream destination_file(destination / (type_decl.get_name() + ".class"), std::ios::binary);

    if (!destination_file)
    {
        error::blasphemy_collector().add_blasphemy("לֹא נִתַּן לִפְתֹּחַ אֶת הַקּוֹבֶץ פֶּלֶט",
            error::blasphemy_type::UNKNOWN, std::nullopt, true);
    }


    write_bytes(destination_file, class_file.magic, 4);
    write_bytes(destination_file, class_file.minor_version, 2);
    write_bytes(destination_file, class_file.major_version, 2);
    write_bytes(destination_file, class_file.constant_pool_count, 2);

    write_constant_pool(destination_file, class_file);

    write_bytes(destination_file, class_file.access_flags, 2);
    write_bytes(destination_file, class_file.this_class, 2);
    write_bytes(destination_file, class_file.super_class, 2);

    write_bytes(destination_file, class_file.interfaces_count, 2);
    write_bytes(destination_file, class_file.fields_count, 2);
    write_bytes(destination_file, class_file.methods_count, 2);

    write_methods(destination_file, class_file.methods_info);

    write_bytes(destination_file, class_file.attribute_count, 2);
    write_attributes(destination_file, class_file.attribute_info);

    destination_file.close();
}

static void write_bytes(std::ofstream &out, const unsigned char *data, const std::streamsize length)
{
    out.write(reinterpret_cast<const char *>(data), length);
}

static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file)
{
    for (const auto &info : class_file.constant_pool)
    {
        write_bytes(out, &info.get().tag, 1);

        if (const auto utf8_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Utf8_info *>(&info.get()))
        {
            write_bytes(out, utf8_info->length, 2);
            write_bytes(out, utf8_info->bytes.data(), utf8_info->bytes.size()); // NOLINT(*-narrowing-conversions) (Already checked in add_utf8_info)
        }
        else if (const auto cls_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Class_info *>(&info.get()))
        {
            write_bytes(out, cls_info->name_index, 2);
        }
        else if (const auto mref_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Methodref_info *>(&info.get()))
        {
            write_bytes(out, mref_info->class_index, 2);
            write_bytes(out, mref_info->name_and_type_index, 2);
        }
        else if (const auto nat_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_NameAndType_info *>(&info.get()))
        {
            write_bytes(out, nat_info->name_index, 2);
            write_bytes(out, nat_info->descriptor_index, 2);
        }
        else
        {
            throw std::runtime_error("Unknown constant pool type");
        }
    }
}

static void write_attributes(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes)
{
    for (const auto &attr : attributes)
    {
        write_bytes(out, attr->attribute_name_index, 2);
        write_bytes(out, attr->attribute_length, 4);

        if (const auto code_attr = dynamic_cast<const codesh::output::jvm_target::defs::code_attribute_entry *>(attr.get()))
        {
            write_bytes(out, code_attr->max_stack, 2);
            write_bytes(out, code_attr->max_locals, 2);
            write_bytes(out, code_attr->code_length, 4);
            write_bytes(out, code_attr->code.data(), code_attr->code.size()); // NOLINT(*-narrowing-conversions)
            write_bytes(out, code_attr->exception_table_length, 2);

            write_bytes(out, code_attr->attribute_count, 2);
            write_attributes(out, code_attr->attributes);
        }
        else if (const auto lnt_attr = dynamic_cast<const codesh::output::jvm_target::defs::line_number_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lnt_attr->line_number_table_length, 2);
            for (const auto &entry : lnt_attr->line_number_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->line_number, 2);
            }
        }
        else if (const auto lvt_attr = dynamic_cast<const codesh::output::jvm_target::defs::local_variable_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lvt_attr->local_variable_table_length, 2);
            for (const auto &entry : lvt_attr->local_variable_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->length, 2);
                write_bytes(out, entry->name_index, 2);
                write_bytes(out, entry->descriptor_index, 2);
                write_bytes(out, entry->index, 2);
            }
        }
        else if (const auto src_attr = dynamic_cast<const codesh::output::jvm_target::defs::source_file_attribute_entry *>(attr.get()))
        {
            write_bytes(out, src_attr->sourcefile_index, 2);
        }
        else
        {
            throw std::runtime_error("Unknown attribute type");
        }
    }
}

static void write_methods(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods)
{
    for (const auto &method : methods)
    {
        write_bytes(out, method->access_flags, 2);
        write_bytes(out, method->name_index, 2);
        write_bytes(out, method->descriptor_index, 2);
        write_bytes(out, method->attributes_count, 2);
        write_attributes(out, method->attribute_info);
    }
}
