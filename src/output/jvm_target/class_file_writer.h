#pragma once

#include <filesystem>

namespace codesh::ast
{
namespace type_decl
{
class type_declaration_ast_node;
}
class compilation_unit_ast_node;
}


namespace codesh::output::jvm_target
{

namespace defs
{
struct class_file;
}


void write_to_file(const defs::class_file &class_file, const ast::compilation_unit_ast_node &root_node,
        const ast::type_decl::type_declaration_ast_node &type_decl,
        const std::filesystem::path &destination);

}
