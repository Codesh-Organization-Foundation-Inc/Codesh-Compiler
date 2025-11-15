#include "class_file_builder.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../util.h"

#include "./defs/attribute_info_entry.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <list>
#include <ranges>

static void write_bytes(std::ofstream &out, const unsigned char *data, std::streamsize length);
static void write_methods(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods);
static void write_attributes(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes);
static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file);


codesh::output::jvm_target::class_file_builder::class_file_builder(const ast::compilation_unit_ast_node &root_node) :
    class_file(),
    root_node(root_node)
{}

codesh::output::jvm_target::defs::class_file codesh::output::jvm_target::class_file_builder::build()
{
    util::put_bytes(class_file.magic, {0xCA, 0xFE, 0xBA, 0xBE});
    util::put_int_bytes(class_file.minor_version, 2, 0);
    util::put_int_bytes(class_file.major_version, 2, JAVA_TARGET_VERSION);

    add_constant_pool_entries();


    add_access_flags({access_flag::ACC_SUPER, access_flag::ACC_PUBLIC});

    util::put_int_bytes(class_file.this_class, 2, 7);
    util::put_int_bytes(class_file.super_class, 2, 2);

    util::put_int_bytes(class_file.interfaces_count, 2, 0);
    util::put_int_bytes(class_file.fields_count, 2, 0);

    util::put_int_bytes(class_file.methods_count, 2, 1);
    add_method();

    util::put_int_bytes(class_file.attribute_count, 2, 1);


    add_source_file();


    return class_file;
}

void codesh::output::jvm_target::class_file_builder::add_constant_pool_entries()
{
    const constant_pool &constant_pool = root_node.get_constant_pool()->get();
    const size_t constant_pool_size = constant_pool.size() + 1;

    if (constant_pool_size > 0xFFFF)
        throw std::runtime_error("Too many constant pool entries; max amount is 65535");

    util::put_int_bytes(class_file.constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)


    for (const auto &constant_pool_entry : constant_pool.get_literals())
    {
        class_file.constant_pool.push_back(constant_pool_entry);
    }
}


void codesh::output::jvm_target::class_file_builder::add_method()
{
    auto method_entry = std::make_unique<defs::methods_info_entry>();

    util::put_int_bytes(method_entry->access_flags, 2, 1);
    util::put_int_bytes(method_entry->name_index, 2, 5);
    util::put_int_bytes(method_entry->descriptor_index, 2, 6);
    util::put_int_bytes(method_entry->attributes_count, 2, 1);


    // Code
    auto code_attr = std::make_unique<defs::code_attribute_entry>();

    util::put_int_bytes(code_attr->attribute_name_index, 2, 9);
    util::put_int_bytes(code_attr->attribute_length, 4, 35);
    util::put_int_bytes(code_attr->max_stack, 2, 1);
    util::put_int_bytes(code_attr->max_locals, 2, 1);
    util::put_int_bytes(code_attr->code_length, 4, 5);

    util::put_bytes(code_attr->code, {0x2A, 0xB7, 0x00, 0x01, 0xB1});

    util::put_int_bytes(code_attr->exception_table_length, 2, 0);
    util::put_int_bytes(code_attr->attribute_count, 2, 1);

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
    util::put_int_bytes(local_variable_table->attribute_name_index, 2, 10);
    util::put_int_bytes(local_variable_table->attribute_length, 4, 12);
    util::put_int_bytes(local_variable_table->local_variable_table_length, 2, 1);

    auto lvt_entry = std::make_unique<defs::local_variable_table_entry>();
    util::put_int_bytes(lvt_entry->start_pc, 2, 0);
    util::put_int_bytes(lvt_entry->length, 2, 5);
    util::put_int_bytes(lvt_entry->name_index, 2, 11);
    util::put_int_bytes(lvt_entry->descriptor_index, 2, 12);
    util::put_int_bytes(lvt_entry->index, 2, 0);
    local_variable_table->local_variable_table.push_back(std::move(lvt_entry));

    code_attr->attributes.push_back(std::move(local_variable_table));


    method_entry->attribute_info.push_back(std::move(code_attr));
    class_file.methods_info.push_back(std::move(method_entry));
}

void codesh::output::jvm_target::class_file_builder::add_source_file()
{
    auto source_file_entry = std::make_unique<defs::source_file_attribute_entry>();
    util::put_int_bytes(source_file_entry->attribute_name_index, 2, 13);
    util::put_int_bytes(source_file_entry->attribute_length, 4, 2);
    util::put_int_bytes(source_file_entry->sourcefile_index, 2, 14);

    class_file.attribute_info.push_back(std::move(source_file_entry));
}

void codesh::output::jvm_target::class_file_builder::add_access_flags(
        const std::list<access_flag> &flags)
{
    //TODO: Change default values
    uint16_t value = 0;

    for (auto flag: flags)
    {
        value |= static_cast<uint16_t>(flag);
    }

    util::put_int_bytes(class_file.access_flags, 2, value);
}

void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file,
    const ast::compilation_unit_ast_node &root_node, const std::filesystem::path &destination)
{
    std::ofstream destination_file(destination / (root_node.get_source_stem() + ".class"), std::ios::binary);

    if (!destination_file)
        throw std::runtime_error("Unable to open output file");


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
        const unsigned char tag[] = {info.get().tag};
        write_bytes(out, tag, 1);

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
            write_bytes(out, code_attr->code, 5);
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
