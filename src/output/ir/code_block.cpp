#include "code_block.h"

#include "../../parser/ast/method/method_declaration_ast_node.h"

const std::list<std::unique_ptr<codesh::output::ir::instruction>> &codesh::output::ir::code_block::get_instructions()
    const
{
    return instructions;
}

void codesh::output::ir::code_block::add_instruction(std::unique_ptr<instruction> instruction)
{
    instructions.emplace_back(std::move(instruction));
}
