#pragma once

#include "../../../defenition/visibility.h"
#include "../impl/ast_node.h"

#include <memory>
#include <vector>

namespace codesh::output::jvm_target
{
enum class access_flag : unsigned short int;
}
namespace codesh::ast::type_decl
{

class attributes_ast_node final : public impl::ast_node
{
    definition::visibility visibility;
    bool is_static;
    bool is_final;
    bool is_abstract;

public:
    attributes_ast_node();

    [[nodiscard]] std::unique_ptr<attributes_ast_node> clone() const;


    [[nodiscard]] std::vector<output::jvm_target::access_flag> get_access_flags() const;

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
