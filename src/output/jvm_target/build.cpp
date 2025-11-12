#include "build.h"


static void add_utf8_info(codesh::output::jvm_target::defs::class_file &cf, const std::string &s)
{
    auto const_utf8 = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>();
    const_utf8->tag[0] = 0x01;
    const_utf8->length[0] = s.size(); // length as 1 byte
    // TODO: Add a check: if size > 255
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
    auto const_methodref = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>();
    const_methodref->tag[0] = 0x0A;
    const_methodref->class_index[0] = (name_index >> 8) & 0xFF;
    const_methodref->class_index[1] = name_index & 0xFF;
    const_methodref->name_and_type_index[0] = (descriptor_index >> 8) & 0xFF;
    const_methodref->name_and_type_index[1] = descriptor_index & 0xFF;
    cf.constant_pool.push_back(std::move(const_methodref));
}

static void add_class_info(codesh::output::jvm_target::defs::class_file &cf, const int name_index)
{
    auto const_class = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Class_info>();
    const_class->tag[0] = 0x0A;
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

void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file, std::string destination)
{
    // TODO: Eliran
}