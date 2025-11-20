#pragma once
#include "defs/class_file.h"

#include <filesystem>
#include <list>

namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
}
namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::output::jvm_target
{
class constant_pool;
}
namespace codesh::ast
{
class method_declaration_ast_node;
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target
{

constexpr int JAVA_TARGET_VERSION = 65;

enum class access_flag : uint16_t
{
    ACC_PUBLIC      = 0x0001,
    ACC_PRIVATE     = 0x0002,
    ACC_PROTECTED   = 0x0004,
    ACC_STATIC      = 0x0008,
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
    const ast::type_decl::type_declaration_ast_node &type_decl;

    const constant_pool &constant_pool_;

    // Common pools
    const int this_class_cpi;
    const int super_class_cpi;


    void handle_class_type(const ast::type_decl::class_declaration_ast_node &class_decl) const;


    void add_constant_pool_entries() const;
    void add_method(const ast::method_declaration_ast_node &method_decl) const;
    void add_source_file() const;

    static void set_access_flags(unsigned char buffer[], const std::vector<access_flag> &flags);

public:
    class_file_builder(const ast::compilation_unit_ast_node &root_node,
            const ast::type_decl::type_declaration_ast_node &type_decl);

    [[nodiscard]] std::unique_ptr<defs::class_file> build();
};

}