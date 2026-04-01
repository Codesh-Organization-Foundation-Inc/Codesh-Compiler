#include "new_array_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/anewarray_instruction.h"
#include "output/ir/instruction/multianewarray_instruction.h"
#include "output/ir/instruction/new_array_instruction.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

using namespace codesh::ast::op;

new_array_ast_node::new_array_ast_node(const lexer::code_position code_position,
        std::unique_ptr<type::type_ast_node> element_type) :
    value_ast_node(code_position),
    element_type(std::move(element_type))
{
}

void new_array_ast_node::add_dimension(std::unique_ptr<value_ast_node> size_expr)
{
    dimensions.push_back(std::move(size_expr));
    element_type->set_array_dimensions(static_cast<int>(dimensions.size()));
}

codesh::ast::type::type_ast_node &new_array_ast_node::get_element_type() const
{
    return *element_type;
}

const std::vector<std::unique_ptr<codesh::ast::var_reference::value_ast_node>> &new_array_ast_node::get_dimensions()
    const
{
    return dimensions;
}


codesh::ast::type::type_ast_node *new_array_ast_node::get_type() const
{
    return element_type.get();
}


void new_array_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    for (const auto &dim : dimensions)
    {
        dim->emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    const auto &cp = containing_type_decl.get_constant_pool();
    const auto *prim = dynamic_cast<const type::primitive_type_ast_node *>(element_type.get());

    if (dimensions.size() == 1)
    {
        if (prim != nullptr)
        {
            containing_block.add_instruction(
                std::make_unique<output::ir::new_array_instruction>(prim->get_type())
            );
        }
        else
        {
            const auto *type = static_cast<const type::custom_type_ast_node *>(element_type.get()); // NOLINT(*-pro-type-static-cast-downcast)
            const int class_cpi = cp.get_class_index(
                cp.get_utf8_index(type->get_resolved_name().join())
            );
            containing_block.add_instruction(
                std::make_unique<output::ir::anewarray_instruction>(class_cpi)
            );
        }
    }
    else
    {
        const int class_cpi = cp.get_class_index(
            cp.get_utf8_index(element_type->generate_descriptor(true))
        );
        containing_block.add_instruction(
            std::make_unique<output::ir::multianewarray_instruction>(
                class_cpi,
                static_cast<unsigned char>(dimensions.size())
            )
        );
    }
}
