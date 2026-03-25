#pragma once

#include "blasphemy/blasphemy_consumer.h"

namespace codesh::ast
{
class compilation_unit_ast_node;
}


namespace codesh::semantic_analyzer
{
class symbol_table;
class country_symbol;

struct semantic_context
{
private:
    const blasphemy::blasphemy_consumer blasphemy_consumer;

public:
    semantic_context(const symbol_table &symbol_table_,
            std::vector<std::reference_wrapper<country_symbol>> lookup_countries,
            const ast::compilation_unit_ast_node &root,
            blasphemy::blasphemy_consumer blasphemy_consumer);

    const symbol_table &symbol_table_;
    const std::vector<std::reference_wrapper<country_symbol>> lookup_countries;
    const ast::compilation_unit_ast_node &root;

    void throw_blasphemy(std::string details, lexer::code_position code_pos) const;
    void throw_blasphemy(std::string details, const lexer::code_range &source_range) const;

    [[nodiscard]] semantic_context with_consumer(blasphemy::blasphemy_consumer blasphemy_consumer) const;

    /**
     * Constructs a new inner semantic context.
     * Thrown blasphemies will have their wrapper names appended.
     */
    [[nodiscard]] semantic_context with_consumer(const std::string &wrapper_name, const std::string &object_name) const;
};

}
