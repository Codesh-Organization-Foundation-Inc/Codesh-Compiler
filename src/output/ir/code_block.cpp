#include "code_block.h"

#include "parser/ast/method/method_declaration_ast_node.h"


codesh::output::ir::code_block::code_block(): is_consuming(false)
{
}

size_t codesh::output::ir::code_block::size() const
{
    size_t result = 0;

    for (const auto &instruction : instructions)
    {
        result += instruction->size();
    }

    return result;
}

const std::vector<std::unique_ptr<codesh::output::ir::instruction>> &codesh::output::ir::code_block::get_instructions()
    const
{
    return instructions;
}

void codesh::output::ir::code_block::add_instruction(std::unique_ptr<instruction> instruction)
{
    instructions.emplace_back(std::move(instruction));
}

bool codesh::output::ir::code_block::get_is_consuming() const
{
    return is_consuming;
}

void codesh::output::ir::code_block::set_is_consuming(const bool is_consuming)
{
    this->is_consuming = is_consuming;
}

void codesh::output::ir::code_block::consume_code_block(code_block block)
{
    for (auto &instruction : block.instructions)
    {
        add_instruction(std::move(instruction));
    }
}
