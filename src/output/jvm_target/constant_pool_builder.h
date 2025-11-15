#pragma once
#include <string>
#include <unordered_map>

namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target::defs
{
struct class_file;
}


namespace codesh::output::jvm_target::constant_pool_builder
{
[[nodiscard]] std::unordered_map<std::string, int> build(const ast::compilation_unit_ast_node &root_node);
}