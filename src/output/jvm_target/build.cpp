#include "build.h"

#include <fstream>
#include <iostream>
#include <list>
#include "./defs/attribute_info_entry.h"

static void add_utf8_info(codesh::output::jvm_target::defs::class_file &class_file, const std::string &str);
static void add_methodref_info(codesh::output::jvm_target::defs::class_file &class_file, int class_index,
                               int name_and_type_index);
static void add_name_and_type_info(codesh::output::jvm_target::defs::class_file &class_file, int name_index,
                                   int descriptor_index);
static void add_class_info(codesh::output::jvm_target::defs::class_file &class_file, int name_index);

static void add_flags(codesh::output::jvm_target::defs::class_file &class_file, const std::list<codesh::output::jvm_target::AccessFlags> &flags);
static void put_bytes(unsigned char arr[], const std::vector<unsigned char> &contents);

/**
 * Puts the number `num` into the array `arr` over `width` bytes.
 *
 * Uses big-endian order (as JVM uses it).
 */
static void put_int_bytes(unsigned char arr[], size_t width, int num);

void write_bytes(std::ofstream &out, const unsigned char *data, std::streamsize length);
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


    { // TODO: add a function that does it
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
    }

    put_int_bytes(class_file.constant_pool_count, 2, class_file.constant_pool.size() + 1);

    add_flags(class_file, {AccessFlags::ACC_SUPER, AccessFlags::ACC_PUBLIC});

    put_int_bytes(class_file.this_class, 2, 7);
    put_int_bytes(class_file.super_class, 2, 2);

    put_int_bytes(class_file.interfaces_count, 2, 0);
    put_int_bytes(class_file.fields_count, 2, 0);

    put_int_bytes(class_file.methods_count, 2, 1);


    auto method_entry = std::make_unique<defs::methods_info_entry>();

    {
        put_int_bytes(method_entry->access_flags, 2, 1);
        put_int_bytes(method_entry->name_index, 2, 5);
        put_int_bytes(method_entry->descriptor_index, 2, 6);
        put_int_bytes(method_entry->attributes_count, 2, 1);

        auto code_attr = std::make_unique<defs::code_attribute_entry>();

        put_int_bytes(code_attr->attribute_name_index, 2, 9);
        put_int_bytes(code_attr->attribute_length, 4, 47);
        put_int_bytes(code_attr->max_stack, 2, 1);
        put_int_bytes(code_attr->max_locals, 2, 1);
        put_int_bytes(code_attr->code_length, 4, 5);

        put_bytes(code_attr->code, {0x2A, 0xB7, 0x00, 0x01, 0xB1});

        put_int_bytes(code_attr->exception_table_length, 2, 0);
        put_int_bytes(code_attr->attribute_count, 2, 2);

        auto line_number_table_attr = std::make_unique<defs::line_number_table_attribute_entry>();

        put_int_bytes(line_number_table_attr->attribute_name_index, 2, 10);
        put_int_bytes(line_number_table_attr->attribute_length, 4, 6);
        put_int_bytes(line_number_table_attr->line_number_table_length, 2, 1);

        auto lnt_entry = std::make_unique<defs::line_number_table_entry>();

        put_int_bytes(lnt_entry->start_pc, 2, 0);
        put_int_bytes(lnt_entry->line_number, 2, 1);

        line_number_table_attr->line_number_table.push_back(std::move(lnt_entry));
        code_attr->attributes.push_back(std::move(line_number_table_attr));


        auto local_variable_table = std::make_unique<defs::local_variable_table_attribute_entry>();

        put_int_bytes(local_variable_table->attribute_name_index, 2, 11);
        put_int_bytes(local_variable_table->attribute_length, 4, 12);
        put_int_bytes(local_variable_table->local_variable_table_length, 2, 1);

        auto lvt_entry = std::make_unique<defs::local_variable_table_entry>();

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

    put_int_bytes(class_file.attribute_count, 2, 1);


    auto source_file_entry = std::make_unique<defs::source_file_attribute_entry>();
    put_int_bytes(source_file_entry->attribute_name_index, 2, 14);
    put_int_bytes(source_file_entry->attribute_length, 4, 2);
    put_int_bytes(source_file_entry->sourcefile_index, 2, 15);

    class_file.attribute_info.push_back(std::move(source_file_entry));


    return class_file;

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
        throw std::runtime_error("String size is longer than possible");

    auto const_utf8 = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>();

    put_int_bytes(const_utf8->tag, 1, 1);
    put_int_bytes(const_utf8->length, 2, str.size()); // NOLINT(*-narrowing-conversions) (Handled overflow above)
    const_utf8->bytes.insert(const_utf8->bytes.end(), str.begin(), str.end());

    class_file.constant_pool.push_back(std::move(const_utf8));
}

static void add_methodref_info(codesh::output::jvm_target::defs::class_file &class_file, const int class_index, const int name_and_type_index)
{
    auto const_methodref = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>();

    put_int_bytes(const_methodref->tag, 1, 10);
    put_int_bytes(const_methodref->class_index, 2, class_index);
    put_int_bytes(const_methodref->name_and_type_index, 2, name_and_type_index);

    class_file.constant_pool.push_back(std::move(const_methodref));
}

static void add_name_and_type_info(codesh::output::jvm_target::defs::class_file &class_file, const int name_index, const int descriptor_index)
{
    auto const_name_and_type = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>();

    put_int_bytes(const_name_and_type->tag, 1, 12);
    put_int_bytes(const_name_and_type->name_index, 2, name_index);
    put_int_bytes(const_name_and_type->descriptor_index, 2, descriptor_index);

    class_file.constant_pool.push_back(std::move(const_name_and_type));
}

static void add_class_info(codesh::output::jvm_target::defs::class_file &class_file, const int name_index)
{
    auto const_class = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>();

    put_int_bytes(const_class->tag, 1, 7);
    put_int_bytes(const_class->name_index, 2, name_index);

    class_file.constant_pool.push_back(std::move(const_class));
}

static void add_flags(codesh::output::jvm_target::defs::class_file &class_file,
                      const std::list<codesh::output::jvm_target::AccessFlags> &flags)
{
    //TODO: Change default values
    uint16_t value = 0;

    for (auto flag: flags)
    {
        value |= static_cast<uint16_t>(flag);
    }

    put_int_bytes(class_file.access_flags, 2, value);
}

void write_bytes(std::ofstream &out, const unsigned char *data, const std::streamsize length)
{
    out.write(reinterpret_cast<const char *>(data), length);
}

static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file)
{
    for (const auto &entry : class_file.constant_pool)
    {
        const auto tag = entry->tag[0];
        write_bytes(out, entry->tag, 1);

        switch (tag)
        {
        case 1: // utf8
        {
            const auto utf8 = static_cast<const codesh::output::jvm_target::defs::CONSTANT_Utf8_info *>(entry.get()); // NOLINT(*-pro-type-static-cast-downcast)
            write_bytes(out, utf8->length, 2);
            write_bytes(out, utf8->bytes.data(), utf8->bytes.size()); // NOLINT(*-narrowing-conversions) (Already checked at add_utf8_info)
            break;
        }
        case 7: // class
        {
            const auto cls = static_cast<const codesh::output::jvm_target::defs::CONSTANT_Class_info *>(entry.get()); // NOLINT(*-pro-type-static-cast-downcast)
            write_bytes(out, cls->name_index, 2);
            break;
        }
        case 10: // methodref
        {
            const auto mref = static_cast<const codesh::output::jvm_target::defs::CONSTANT_Methodref_info *>(entry.get()); // NOLINT(*-pro-type-static-cast-downcast)
            write_bytes(out, mref->class_index, 2);
            write_bytes(out, mref->name_and_type_index, 2);
            break;
        }
        case 12: // name_and_type
        {
            const auto nat = static_cast<const codesh::output::jvm_target::defs::CONSTANT_NameAndType_info *>(entry.get()); // NOLINT(*-pro-type-static-cast-downcast)
            write_bytes(out, nat->name_index, 2);
            write_bytes(out, nat->descriptor_index, 2);
            break;
        }
        default:
            std::cerr << "Unknown constant pool tag: " << static_cast<int>(tag) << std::endl;
            break;
        }
    }
}

static void write_attributes(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes)
{
    for (const auto &attr : attributes)
    {
        write_bytes(out, attr->attribute_name_index, 2);
        write_bytes(out, attr->attribute_length, 4);

        if (const auto code = dynamic_cast<const codesh::output::jvm_target::defs::code_attribute_entry *>(attr.get()))
        {
            write_bytes(out, code->max_stack, 2);
            write_bytes(out, code->max_locals, 2);
            write_bytes(out, code->code_length, 4);
            write_bytes(out, code->code, 5);
            write_bytes(out, code->exception_table_length, 2);

            write_bytes(out, code->attribute_count, 2);
            write_attributes(out, code->attributes);
        }
        else if (const auto lnt = dynamic_cast<const codesh::output::jvm_target::defs::line_number_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lnt->line_number_table_length, 2);
            for (const auto &entry : lnt->line_number_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->line_number, 2);
            }
        }
        else if (const auto lvt = dynamic_cast<const codesh::output::jvm_target::defs::local_variable_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lvt->local_variable_table_length, 2);
            for (const auto &entry : lvt->local_variable_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->length, 2);
                write_bytes(out, entry->name_index, 2);
                write_bytes(out, entry->descriptor_index, 2);
                write_bytes(out, entry->index, 2);
            }
        }
        else if (const auto src = dynamic_cast<const codesh::output::jvm_target::defs::source_file_attribute_entry *>(attr.get()))
        {
            write_bytes(out, src->sourcefile_index, 2);
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