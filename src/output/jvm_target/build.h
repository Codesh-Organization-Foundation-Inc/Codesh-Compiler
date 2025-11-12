#pragma once
#include "defs/class_file.h"

namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target
{

constexpr int JAVA_TARGET_VERSION = 65;

[[nodiscard]] defs::class_file build(const ast::compilation_unit_ast_node *root_node);
void write_to_file(const defs::class_file &class_file, std::string destination);

}