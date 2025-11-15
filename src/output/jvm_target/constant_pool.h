#pragma once

#include "defs/cp_info.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace codesh::output::jvm_target::defs
{
class cp_info;
}
namespace codesh::ast::type_decl
{
class class_declaration_ast_node;
}
namespace codesh::ast
{
class compilation_unit_ast_node;
}
namespace codesh::output::jvm_target::defs
{
struct class_file;
}


namespace codesh::output::jvm_target
{
class constant_pool
{
    std::unordered_map<std::string, int> string_literals;
    std::unordered_map<std::unique_ptr<const defs::cp_info>, int, defs::cp_info_ptr_hash, defs::cp_info_ptr_equal> literals;

    void collect_literals(const ast::compilation_unit_ast_node &root_node);
    void collect_non_literals(const ast::compilation_unit_ast_node &root_node);

    static void traverse_type_decls(const ast::compilation_unit_ast_node &root_node,
            std::unordered_set<std::string> &literals);
    static void traverse_class_decl(const ast::type_decl::class_declaration_ast_node &root_node,
            std::unordered_set<std::string> &literals);

public:
    /**
     * Constructs a new constant pool using the provided AST node
     */
    explicit constant_pool(const ast::compilation_unit_ast_node &root_node);

    [[nodiscard]] int get_index(const std::string &literal) const;

    /**
     * @return All string literals by the order of their index
     */
    [[nodiscard]] std::vector<std::string> get_string_literals() const;
};
}