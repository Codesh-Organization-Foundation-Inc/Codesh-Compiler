#include "util.h"

#include <algorithm>

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"

bool codesh::semantic_analyzer::util::type_exists(const ast::compilation_unit_ast_node &root, const std::string &name)
{
    return std::ranges::any_of(
        //FIXME: This should point to the symbol table
        root.get_type_declarations(),

        [&name](auto &declaration) {
            const auto *class_declaration = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(
                declaration.get()
            );

            if (class_declaration)
                return class_declaration->get_name() == name;

            return false;
        }
    );
}