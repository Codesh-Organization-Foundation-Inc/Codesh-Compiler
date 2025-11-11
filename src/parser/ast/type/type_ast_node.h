#pragma once

#include "../impl/ast_node.h"
#include "../impl/i_descriptor_emitter.h"

namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class type_ast_node : public impl::ast_node, public impl::i_descriptor_emitter
{
    int array_dimensions;

public:
    type_ast_node();

    [[nodiscard]] int get_array_dimensions() const;
    void set_array_dimensions(int array_dimensions);
};

}
