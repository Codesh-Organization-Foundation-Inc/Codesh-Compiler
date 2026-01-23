#include "code_block.h"

#include "parser/ast/method/method_declaration_ast_node.h"

size_t codesh::output::ir::code_block::size() const
{
    size_t result = 0;

    for (const auto &instruction : instructions)
    {
        result += instruction->size();
    }

    return result;
}

const std::list<std::unique_ptr<codesh::output::ir::instruction>> &codesh::output::ir::code_block::get_instructions()
    const
{
    return instructions;
}

void codesh::output::ir::code_block::add_instruction(std::unique_ptr<instruction> instruction)
{
    instructions.emplace_back(std::move(instruction));
}

void codesh::output::ir::code_block::consume_code_block(code_block block)
{
    for (auto &instruction : block.instructions)
    {
        add_instruction(std::move(instruction));
    }
}
