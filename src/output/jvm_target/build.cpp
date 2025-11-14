#include "build.h"

#include <fstream>
#include <iostream>
#include "./defs/attribute_info_entry.h"

#include <list>

static void add_constant_pool_entries(codesh::output::jvm_target::defs::class_file &class_file);
static void add_method(codesh::output::jvm_target::defs::class_file &class_file);
static void add_source_file(codesh::output::jvm_target::defs::class_file &class_file);

static void add_utf8_info(codesh::output::jvm_target::defs::class_file &class_file, const std::string &str);
static void add_methodref_info(codesh::output::jvm_target::defs::class_file &class_file, int class_index,
                               int name_and_type_index);
static void add_name_and_type_info(codesh::output::jvm_target::defs::class_file &class_file, int name_index,
                                   int descriptor_index);
static void add_class_info(codesh::output::jvm_target::defs::class_file &class_file, int name_index);

static void add_access_flags(codesh::output::jvm_target::defs::class_file &class_file, const std::list<codesh::output::jvm_target::access_flag> &flags);
static void put_bytes(unsigned char arr[], const std::vector<unsigned char> &contents);

/**
 * Puts the number `num` into the array `arr` over `width` bytes.
 *
 * Uses big-endian order (as JVM uses it).
 */
static void put_int_bytes(unsigned char arr[], size_t width, int num);

static void write_bytes(std::ofstream &out, const unsigned char *data, std::streamsize length);
static void write_methods(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods);
static void write_attributes(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes);
static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file);


codesh::output::jvm_target::defs::class_file codesh::output::jvm_target::build(
    const ast::compilation_unit_ast_node *root_node)
{
    defs::class_file class_file {
        .magic = {0xCA, 0xFE, 0xBA, 0xBE},
    };

    put_int_bytes(class_file.minor_version, 2, 0);
    put_int_bytes(class_file.major_version, 2, JAVA_TARGET_VERSION);

    add_constant_pool_entries(class_file);


    add_access_flags(class_file, {access_flag::ACC_SUPER, access_flag::ACC_PUBLIC});

    put_int_bytes(class_file.this_class, 2, 7);
    put_int_bytes(class_file.super_class, 2, 2);

    put_int_bytes(class_file.interfaces_count, 2, 0);
    put_int_bytes(class_file.fields_count, 2, 0);

    put_int_bytes(class_file.methods_count, 2, 1);
    add_method(class_file);

    put_int_bytes(class_file.attribute_count, 2, 1);


    add_source_file(class_file);


    return class_file;
}

static void add_constant_pool_entries(codesh::output::jvm_target::defs::class_file &class_file)
{
    add_methodref_info(class_file, 2, 3);
    add_class_info(class_file, 4);
    add_name_and_type_info(class_file, 5, 6);
    add_utf8_info(class_file, "java/lang/Object");
    add_utf8_info(class_file, "<init>");
    add_utf8_info(class_file, "()V");
    add_class_info(class_file, 8);
    add_utf8_info(class_file, "Main");
    add_utf8_info(class_file, "Code");
    add_utf8_info(class_file, "LineNumberTable");
    add_utf8_info(class_file, "LocalVariableTable");
    add_utf8_info(class_file, "this");
    add_utf8_info(class_file, "LMain;");
    add_utf8_info(class_file, "SourceFile");
    add_utf8_info(class_file, "Main.java");


    //TODO: Put this block at the beginning when no mock values are implemented.
    // This is as the constant_pool_size should be known beforehand.

    const size_t constant_pool_size = class_file.constant_pool.size() + 1;

    if (constant_pool_size > 0xFFFF)
        throw std::runtime_error("Too many constant pool entries; max amount is 65535");

    put_int_bytes(class_file.constant_pool_count, 2, constant_pool_size); // NOLINT(*-narrowing-conversions) (Checked overflow above)
}

static void add_method(codesh::output::jvm_target::defs::class_file &class_file)
{
    auto method_entry = std::make_unique<codesh::output::jvm_target::defs::methods_info_entry>();

    put_int_bytes(method_entry->access_flags, 2, 1);
    put_int_bytes(method_entry->name_index, 2, 5);
    put_int_bytes(method_entry->descriptor_index, 2, 6);
    put_int_bytes(method_entry->attributes_count, 2, 1);


    // Code
    auto code_attr = std::make_unique<codesh::output::jvm_target::defs::code_attribute_entry>();

    put_int_bytes(code_attr->attribute_name_index, 2, 9);
    put_int_bytes(code_attr->attribute_length, 4, 47);
    put_int_bytes(code_attr->max_stack, 2, 1);
    put_int_bytes(code_attr->max_locals, 2, 1);
    put_int_bytes(code_attr->code_length, 4, 5);

    put_bytes(code_attr->code, {0x2A, 0xB7, 0x00, 0x01, 0xB1});

    put_int_bytes(code_attr->exception_table_length, 2, 0);
    put_int_bytes(code_attr->attribute_count, 2, 2);

    // Line number table
    auto line_number_table_attr = std::make_unique<codesh::output::jvm_target::defs::line_number_table_attribute_entry>();
    put_int_bytes(line_number_table_attr->attribute_name_index, 2, 10);
    put_int_bytes(line_number_table_attr->attribute_length, 4, 6);
    put_int_bytes(line_number_table_attr->line_number_table_length, 2, 1);

    auto lnt_entry = std::make_unique<codesh::output::jvm_target::defs::line_number_table_entry>();
    put_int_bytes(lnt_entry->start_pc, 2, 0);
    put_int_bytes(lnt_entry->line_number, 2, 1);

    line_number_table_attr->line_number_table.push_back(std::move(lnt_entry));
    code_attr->attributes.push_back(std::move(line_number_table_attr));


    // Local variables
    auto local_variable_table = std::make_unique<codesh::output::jvm_target::defs::local_variable_table_attribute_entry>();
    put_int_bytes(local_variable_table->attribute_name_index, 2, 11);
    put_int_bytes(local_variable_table->attribute_length, 4, 12);
    put_int_bytes(local_variable_table->local_variable_table_length, 2, 1);

    auto lvt_entry = std::make_unique<codesh::output::jvm_target::defs::local_variable_table_entry>();
    put_int_bytes(lvt_entry->start_pc, 2, 0);
    put_int_bytes(lvt_entry->length, 2, 5);
    put_int_bytes(lvt_entry->name_index, 2, 12);
    put_int_bytes(lvt_entry->descriptor_index, 2, 13);
    put_int_bytes(lvt_entry->index, 2, 0);
    local_variable_table->local_variable_table.push_back(std::move(lvt_entry));

    code_attr->attributes.push_back(std::move(local_variable_table));


    method_entry->attribute_info.push_back(std::move(code_attr));
    class_file.methods_info.push_back(std::move(method_entry));
}

static void add_source_file(codesh::output::jvm_target::defs::class_file &class_file)
{
    auto source_file_entry = std::make_unique<codesh::output::jvm_target::defs::source_file_attribute_entry>();
    put_int_bytes(source_file_entry->attribute_name_index, 2, 14);
    put_int_bytes(source_file_entry->attribute_length, 4, 2);
    put_int_bytes(source_file_entry->sourcefile_index, 2, 15);

    class_file.attribute_info.push_back(std::move(source_file_entry));
}


void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file, const std::string &destination)
{
    std::ofstream destination_file(destination, std::ios::binary);

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

static void put_bytes(unsigned char arr[], const std::vector<unsigned char> &contents)
{
    for (size_t i = 0; i < contents.size(); i++)
    {
        arr[i] = contents[i];
    }
}

static void put_int_bytes(unsigned char arr[], const size_t width, const int num)
{
    for (int i = 0; i < width; i++)
    {
        arr[width - 1 - i] = static_cast<unsigned char>(num >> (8 * i) & 0xFF);
    }
}


static void add_utf8_info(codesh::output::jvm_target::defs::class_file &class_file, const std::string &str)
{
    if (str.size() > 0xFFFF)
        throw std::runtime_error("String size is longer than possible; max length is 65535");

    auto const_utf8 = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>();

    put_int_bytes(const_utf8->length, 2, str.size()); // NOLINT(*-narrowing-conversions) (Handled overflow above)
    const_utf8->bytes.insert(const_utf8->bytes.end(), str.begin(), str.end());

    class_file.constant_pool.push_back(std::move(const_utf8));
}

static void add_methodref_info(codesh::output::jvm_target::defs::class_file &class_file, const int class_index, const int name_and_type_index)
{
    auto const_methodref = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>();

    put_int_bytes(const_methodref->class_index, 2, class_index);
    put_int_bytes(const_methodref->name_and_type_index, 2, name_and_type_index);

    class_file.constant_pool.push_back(std::move(const_methodref));
}

static void add_name_and_type_info(codesh::output::jvm_target::defs::class_file &class_file, const int name_index, const int descriptor_index)
{
    auto const_name_and_type = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>();

    put_int_bytes(const_name_and_type->name_index, 2, name_index);
    put_int_bytes(const_name_and_type->descriptor_index, 2, descriptor_index);

    class_file.constant_pool.push_back(std::move(const_name_and_type));
}

static void add_class_info(codesh::output::jvm_target::defs::class_file &class_file, const int name_index)
{
    auto const_class = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>();

    put_int_bytes(const_class->name_index, 2, name_index);

    class_file.constant_pool.push_back(std::move(const_class));
}

static void add_access_flags(codesh::output::jvm_target::defs::class_file &class_file,
                      const std::list<codesh::output::jvm_target::access_flag> &flags)
{
    //TODO: Change default values
    uint16_t value = 0;

    for (auto flag: flags)
    {
        value |= static_cast<uint16_t>(flag);
    }

    put_int_bytes(class_file.access_flags, 2, value);
}

static void write_bytes(std::ofstream &out, const unsigned char *data, const std::streamsize length)
{
    out.write(reinterpret_cast<const char *>(data), length);
}

static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file)
{
    for (const auto &info : class_file.constant_pool)
    {
        if (const auto utf8_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Utf8_info *>(info.get()))
        {
            write_bytes(out, utf8_info->length, 2);
            write_bytes(out, utf8_info->bytes.data(), utf8_info->bytes.size());
        }
        else if (const auto cls_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Class_info *>(info.get()))
        {
            write_bytes(out, cls_info->name_index, 2);
        }
        else if (const auto mref_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Methodref_info *>(info.get()))
        {
            write_bytes(out, mref_info->class_index, 2);
            write_bytes(out, mref_info->name_and_type_index, 2);
        }
        else if (const auto nat_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_NameAndType_info *>(info.get()))
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

static int operator|(const codesh::output::jvm_target::access_flag lhs, const codesh::output::jvm_target::access_flag rhs)
{
    return static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs);
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
