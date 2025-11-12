#include "build.h"


static void add_utf8(codesh::output::jvm_target::defs::class_file &cf, const std::string &s)
{
    auto const_utf8 = std::make_unique<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>();
    const_utf8->tag[0] = 1;
    const_utf8->length[0] = s.size(); // length as 1 byte
    // TODO: Add a check: if size > 255
    const_utf8->bytes.insert(const_utf8->bytes.end(), s.begin(), s.end());
    cf.constant_pool.push_back(std::move(const_utf8));
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
    class_file.constant_pool_count[0] = 0x10;



}

void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file, std::string destination)
{
    // TODO: Eliran
}