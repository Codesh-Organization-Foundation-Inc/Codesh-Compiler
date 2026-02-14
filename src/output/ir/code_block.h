#pragma once

#include "instruction/impl/instruction.h"

#include <memory>
#include <vector>

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
    std::vector<std::unique_ptr<instruction>> instructions;
    bool is_consuming;

public:
    code_block();
    [[nodiscard]] size_t size() const;

    [[nodiscard]] const std::vector<std::unique_ptr<instruction>> &get_instructions() const;
    void add_instruction(std::unique_ptr<instruction> instruction);

    [[nodiscard]] bool get_is_consuming() const;
    void set_is_consuming(bool is_consuming);

    void consume_code_block(code_block block);
};

}
