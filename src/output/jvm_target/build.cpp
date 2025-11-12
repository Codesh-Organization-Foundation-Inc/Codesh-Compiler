#include "build.h"


static void add_utf8_info(codesh::output::jvm_target::defs::class_file &cf, const std::string &s)
{
    auto const_utf8 = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>();
    const_utf8->tag[0] = 0x01;
    const_utf8->length[0] = (s.size() >> 8) & 0xFF;
    const_utf8->length[1] = s.size() & 0xFF;
    // TODO: Add a check: if size > 0xFFFF
    const_utf8->bytes.insert(const_utf8->bytes.end(), s.begin(), s.end());
    cf.constant_pool.push_back(std::move(const_utf8));
}

static void add_methodref_info(codesh::output::jvm_target::defs::class_file &cf, const int class_index, const int name_and_type_index)
{
    auto const_methodref = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>();
    const_methodref->tag[0] = 0x0A;
    const_methodref->class_index[0] = (class_index >> 8) & 0xFF;
    const_methodref->class_index[1] = class_index & 0xFF;
    const_methodref->name_and_type_index[0] = (name_and_type_index >> 8) & 0xFF;
    const_methodref->name_and_type_index[1] = name_and_type_index & 0xFF;
    cf.constant_pool.push_back(std::move(const_methodref));
}

static void add_name_and_type_info(codesh::output::jvm_target::defs::class_file &cf, const int name_index, const int descriptor_index)
{
    auto const_name_and_type = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>();
    const_name_and_type->tag[0] = 0x0C;
    const_name_and_type->name_index[0] = (name_index >> 8) & 0xFF;
    const_name_and_type->name_index[1] = name_index & 0xFF;
    const_name_and_type->descriptor_index[0] = (descriptor_index >> 8) & 0xFF;
    const_name_and_type->descriptor_index[1] = descriptor_index & 0xFF;
    cf.constant_pool.push_back(std::move(const_name_and_type));
}

static void add_class_info(codesh::output::jvm_target::defs::class_file &cf, const int name_index)
{
    auto const_class = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>();
    const_class->tag[0] = 0x07;
    const_class->name_index[0] = (name_index >> 8) & 0xFF;
    const_class->name_index[1] = name_index & 0xFF;
    cf.constant_pool.push_back(std::move(const_class));
}

codesh::output::jvm_target::defs::class_file codesh::output::jvm_target::build(
    const ast::compilation_unit_ast_node *root_node)
{
    defs::class_file class_file;

    // magic
    class_file.magic[0] = 0xCA;
    class_file.magic[1] = 0xFE;
    class_file.magic[2] = 0xBA;
    class_file.magic[3] = 0xBE;

    // minor version
    class_file.minor_version[0] = 0x00;
    class_file.minor_version[1] = 0x00;

    // major version
    class_file.minor_version[0] = 0x00;
    class_file.minor_version[1] = 0x41;

    // constant pool
    class_file.constant_pool_count[0] = 0x00;
    class_file.constant_pool_count[1] = 0x10;

    // TODO: make it that you dont need to add class_file to arguments
    { // TODO: add a function that does it
        add_methodref_info(class_file, 2, 3);
        add_class_info(class_file, 4);
        add_name_and_type_info(class_file, 5, 6);
        add_utf8_info(class_file, "java.lang.Object");
        add_utf8_info(class_file, "<init>");
        add_utf8_info(class_file, "()V");
        add_class_info(class_file, 8);
        add_utf8_info(class_file, "Main");


        add_utf8_info(class_file, "Code");
        add_utf8_info(class_file, "LineNumberTable");
        add_utf8_info(class_file, "LocalVariableTable");
        add_utf8_info(class_file, "this");
        add_utf8_info(class_file, "Lmain;");
        add_utf8_info(class_file, "SourceFile;");
        add_utf8_info(class_file, "Main.java;");
    }

    class_file.constant_pool_count[0] = 0x00;
    class_file.constant_pool_count[1] = 0x21;

    class_file.this_class[0] = 0x00;
    class_file.this_class[1] = 0x07;

    class_file.super_class[0] = 0x00;
    class_file.super_class[1] = 0x02;

    class_file.interfaces_count[0] = 0x00;
    class_file.interfaces_count[1] = 0x00;
    class_file.fields_count[0] = 0x00;
    class_file.fields_count[1] = 0x00;

    class_file.methods_count[0] = 0x00;
    class_file.methods_count[1] = 0x01;

    auto method_entry = std::make_unique<defs::methods_info_entry>();

    {
        method_entry->access_flags[0] = 0x00;
        method_entry->access_flags[1] = 0x01;

        method_entry->name_index[0] = 0x00;
        method_entry->name_index[1] = 0x05;
        method_entry->descriptor_index[0] = 0x00;
        method_entry->descriptor_index[1] = 0x06;

        method_entry->attributes_count[0] = 0x00;
        method_entry->attributes_count[1] = 0x01;

        auto code_attr = std::make_unique<defs::code_attribute_entry>();

        code_attr->attribute_name_index[0] = 0x00;
        code_attr->attribute_name_index[1] = 0x09;

        code_attr->attribute_length[0] = 0x00;
        code_attr->attribute_length[1] = 0x00;
        code_attr->attribute_length[2] = 0x00;
        code_attr->attribute_length[3] = 0x2F;

        code_attr->max_stack[0] = 0x00;
        code_attr->max_stack[1] = 0x01;

        code_attr->max_locals[0] = 0x00;
        code_attr->max_locals[1] = 0x01;

        code_attr->code_length[0] = 0x00;
        code_attr->code_length[1] = 0x00;
        code_attr->code_length[2] = 0x00;
        code_attr->code_length[3] = 0x05;

        unsigned char bc[5] = {0x2A, 0xB7, 0x00, 0x01, 0xB1};
        for (int i = 0; i < 5; i++)
        {
            code_attr->code[i] = bc[i];
        }

        code_attr->exception_table_length[0] = 0x00;
        code_attr->exception_table_length[1] = 0x00;

        code_attr->attribute_count[0] = 0x00;
        code_attr->attribute_count[1] = 0x02;

        auto line_number_table_attr = std::make_unique<defs::line_number_table_attribute_entry>();

        line_number_table_attr->attribute_name_index[0] = 0x00;
        line_number_table_attr->attribute_name_index[1] = 0x0A;

        line_number_table_attr->attribute_length[0] = 0x00;
        line_number_table_attr->attribute_length[1] = 0x00;
        line_number_table_attr->attribute_length[2] = 0x00;
        line_number_table_attr->attribute_length[3] = 0x06;

        line_number_table_attr->line_number_table_length[0] = 0x00;
        line_number_table_attr->line_number_table_length[1] = 0x01;

        auto lnt_entry = std::make_unique<defs::line_number_table_entry>();
        lnt_entry->start_pc[0] = 0x00;
        lnt_entry->start_pc[1] = 0x00;
        lnt_entry->line_number[0] = 0x00;
        lnt_entry->line_number[1] = 0x01;

        line_number_table_attr->line_number_table.push_back(std::move(lnt_entry));

        code_attr->attributes.push_back(std::move(line_number_table_attr));


        auto local_variable_table = std::make_unique<defs::local_variable_table_attribute_entry>();

        local_variable_table->attribute_name_index[0] = 0x00;
        local_variable_table->attribute_name_index[1] = 0x0B;

        local_variable_table->attribute_length[0] = 0x00;
        local_variable_table->attribute_length[1] = 0x00;
        local_variable_table->attribute_length[2] = 0x00;
        local_variable_table->attribute_length[3] = 0x0C;

        local_variable_table->local_variable_table_length[0] = 0x00;
        local_variable_table->local_variable_table_length[1] = 0x01;

        auto lvt_entry = std::make_unique<defs::local_variable_table_entry>();

        lvt_entry->start_pc[0] = 0x00;
        lvt_entry->start_pc[1] = 0x00;
        lvt_entry->length[0] = 0x00;
        lvt_entry->length[1] = 0x05;
        lvt_entry->name_index[0] = 0x00;
        lvt_entry->name_index[1] = 0x0C;
        lvt_entry->descriptor_index[0] = 0x00;
        lvt_entry->descriptor_index[1] = 0x0D;
        lvt_entry->index[0] = 0x00;
        lvt_entry->index[1] = 0x00;

        local_variable_table->local_variable_table.push_back(std::move(lvt_entry));

        code_attr->attributes.push_back(std::move(local_variable_table));

        method_entry->attribute_info.push_back(std::move(code_attr));

        class_file.methods_info.push_back(std::move(method_entry));
    }


    class_file.attribute_count[0] = 0x00;
    class_file.attribute_count[1] = 0x01;

    auto source_file_entry = std::make_unique<defs::source_file_attribute_entry>();
    source_file_entry->attribute_name_index[0] = 0x00;
    source_file_entry->attribute_name_index[1] = 0x0E;

    source_file_entry->attribute_length[0] = 0x00;
    source_file_entry->attribute_length[1] = 0x00;
    source_file_entry->attribute_length[2] = 0x00;
    source_file_entry->attribute_length[3] = 0x02;

    source_file_entry->sourcefile_index[0] = 0x00;
    source_file_entry->sourcefile_index[1] = 0x0F;


    return class_file;

}

void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file, std::string destination)
{
    // TODO: Eliran
}