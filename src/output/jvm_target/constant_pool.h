#pragma once

#include "../../parser/ast/method/operation/method_call_ast_node.h"
#include "defs/cp_info.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
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
namespace method
{
class method_declaration_ast_node;
}
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
    const ast::compilation_unit_ast_node &root_node;
    const ast::type_decl::type_declaration_ast_node &type_decl;

    std::unordered_set<
        std::unique_ptr<const defs::cp_info>,
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

    void traverse_class_decl(const ast::type_decl::class_declaration_ast_node &class_decl);
    void traverse_method_decl(const ast::type_decl::class_declaration_ast_node &class_decl);
    void traverse_method_body(const ast::method::method_declaration_ast_node &method_decl);
    void traverse_method_call(const ast::method::operation::method_call_ast_node &method_call);

    int index;
    const int this_class_cpi;


    // Each of these Get or Creates (GoC) return the index of the constant in the pool (CPI).
    int goc_constant(std::unique_ptr<defs::cp_info> constant_info);

    int goc_utf8_info(const std::string &utf8);
    int goc_string_info(int utf8_index);
    int goc_integer_info(int num);

    int goc_methodref_info(int class_index, int name_and_type_index);
    int goc_name_and_type_info(int name_index, int descriptor_index);
    int goc_class_info(int name_index);


    static std::unique_ptr<defs::CONSTANT_Utf8_info> utf8_info(const std::string &utf8);
    static std::unique_ptr<defs::CONSTANT_String_info> string_info(int utf8_index);
    static std::unique_ptr<defs::CONSTANT_Integer_info> integer_info(int num);

    static std::unique_ptr<defs::CONSTANT_Methodref_info> methodref_info(int class_index, int name_and_type_index);
    static std::unique_ptr<defs::CONSTANT_NameAndType_info> name_and_type_info(int name_index, int descriptor_index);
    static std::unique_ptr<defs::CONSTANT_Class_info> class_info(int name_index);

public:
    /**
     * Constructs a new constant pool using the provided AST node
     */
    constant_pool(const ast::compilation_unit_ast_node &root_node,
            const ast::type_decl::type_declaration_ast_node &type_decl);

    [[nodiscard]] int get_index(const defs::cp_info &literal) const;

    [[nodiscard]] int get_utf8_index(const std::string &utf8) const;
    [[nodiscard]] int get_string_index(int utf8_index) const;
    [[nodiscard]] int get_integer_index(int num) const;

    [[nodiscard]] int get_methodref_index(int class_index, int name_and_type_index) const;
    [[nodiscard]] int get_name_and_type_index(int name_index, int descriptor_index) const;
    [[nodiscard]] int get_class_index(int name_index) const;

    /**
     * @return All string literals by the order of their index
     */
    [[nodiscard]] std::vector<std::reference_wrapper<const defs::cp_info>> get_literals() const;

    [[nodiscard]] size_t size() const;
};
}
