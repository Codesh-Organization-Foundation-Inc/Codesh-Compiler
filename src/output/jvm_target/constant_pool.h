#pragma once

#include <map>
#include <ranges>
#include <string>
#include <unordered_set>

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
    std::map<std::string, int> string_literals;

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

    [[nodiscard]] std::ranges::elements_view<std::ranges::ref_view<const std::map<std::string, int>>, 0>
        get_string_literals() const;

};
}