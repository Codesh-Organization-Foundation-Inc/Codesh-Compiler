#pragma once
#include "defs/class_file.h"

#include <filesystem>
#include <list>

namespace codesh::output::jvm_target
{
class constant_pool;
}
namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target
{

constexpr int JAVA_TARGET_VERSION = 65;

enum class access_flag : uint16_t
{
    ACC_PUBLIC      = 0x0001,
    ACC_FINAL       = 0x0010,
    ACC_SUPER       = 0x0020,
    ACC_INTERFACE   = 0x0200,
    ACC_ABSTRACT    = 0x0400,
    ACC_SYNTHETIC   = 0x1000,
    ACC_ANNOTATION  = 0x2000,
    ACC_ENUM        = 0x4000,
    ACC_MODULE      = 0x8000
};


class class_file_builder
{
    std::unique_ptr<defs::class_file> class_file;

    const ast::compilation_unit_ast_node &root_node;
    const constant_pool &constant_pool_;


    void add_constant_pool_entries() const;
    void add_method() const;
    void add_source_file() const;

    void add_access_flags(const std::list<access_flag> &flags) const;

public:
    explicit class_file_builder(const ast::compilation_unit_ast_node &root_node);

    [[nodiscard]] std::unique_ptr<defs::class_file> build();
};

}