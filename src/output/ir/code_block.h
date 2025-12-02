#pragma once

#include "instruction.h"

#include <list>
#include <memory>

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::ast::method
{
class method_declaration_ast_node;
}


namespace codesh::output::ir
{

class code_block
{
    std::list<std::unique_ptr<instruction>> instructions;

public:
    static code_block build_from_method(const ast::method::method_declaration_ast_node &method,
            const semantic_analyzer::symbol_table &symbol_table,
            const ast::type_decl::type_declaration_ast_node &containing_type);

    [[nodiscard]] const std::list<std::unique_ptr<instruction>> &get_instructions() const;
    void add_instruction(std::unique_ptr<instruction> instruction);
};

}
