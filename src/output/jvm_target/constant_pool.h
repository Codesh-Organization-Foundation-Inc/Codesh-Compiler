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
    std::unordered_map<
        std::unique_ptr<const defs::cp_info>, int,
        defs::cp_info_unique_ptr_hash, defs::cp_info_unique_ptr_equal
    > literals;
    /**
     * C++ is fucking dumb and we can't .find() a fucking reference/pointer when the key is a uNiqUe pOIntEr.
     * fucking goo goo ga ga ahh language
     */
    std::unordered_map<
        const defs::cp_info *, int,
        defs::cp_info_ptr_hash, defs::cp_info_ptr_equal
    > literals_lookup_map;

    void traverse_type_decls(const ast::compilation_unit_ast_node &root_node);
    void traverse_class_decl(const ast::type_decl::class_declaration_ast_node &class_decl_node);


    size_t index;
    void add_constant(std::unique_ptr<defs::cp_info> root_node);

    void add_utf8_constant(const std::string &utf8);
    void add_methodref_info(int class_index, int name_and_type_index);
    void add_name_and_type_info(int name_index, int descriptor_index);
    void add_class_info(int name_index);

public:
    /**
     * Constructs a new constant pool using the provided AST node
     */
    explicit constant_pool(const ast::compilation_unit_ast_node &root_node);

    [[nodiscard]] int get_index(const defs::cp_info &literal) const;

    /**
     * @return All string literals by the order of their index
     */
    [[nodiscard]] std::vector<std::reference_wrapper<const defs::cp_info>> get_literals() const;
};
}
