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
    [[nodiscard]] size_t size() const;

    [[nodiscard]] const std::list<std::unique_ptr<instruction>> &get_instructions() const;
    void add_instruction(std::unique_ptr<instruction> instruction);

    void consume_code_block(code_block block);
};

}
