#pragma once

#include "../../../defenition/visibility.h"
#include "../impl/ast_node.h"

namespace codesh::ast::type
{

class attributes_ast_node final : public impl::ast_node
{
    definition::visibility visibility;
    bool is_static;
    bool is_final;
    bool is_abstract;

public:
    attributes_ast_node();

    [[nodiscard]] definition::visibility get_visibility() const;
    void set_visibility(definition::visibility visibility);

    [[nodiscard]] bool get_is_static() const;
    void set_is_static(bool is_static);

    [[nodiscard]] bool get_is_final() const;
    void set_is_final(bool is_final);

    [[nodiscard]] bool get_is_abstract() const;
    void set_is_abstract(bool is_abstract);

};

}
