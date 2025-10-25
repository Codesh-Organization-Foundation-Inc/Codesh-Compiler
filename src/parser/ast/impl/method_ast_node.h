#pragma once

#include "ast_node.h"

namespace codesh::ast::impl
{

/**
 * An AST node that can be converted to method IR
 */
class method_ast_node : public ast_node
{
    //TODO: Update return type when implemented IR structs etc.
    /**
     * @return The IR representation of this AST node
     */
public:
    [[nodiscard]] virtual void *to_ir() const = 0;
};

}

