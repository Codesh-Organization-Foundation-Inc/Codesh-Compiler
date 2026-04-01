#include "semantic_context.h"

#include "fmt/xchar.h"

codesh::semantic_analyzer::semantic_context::semantic_context(symbol_table &symbol_table_,
        std::vector<std::reference_wrapper<country_symbol>> lookup_countries,
        const ast::compilation_unit_ast_node &root,
        blasphemy::blasphemy_consumer blasphemy_consumer) :
    blasphemy_consumer(std::move(blasphemy_consumer)),
    symbol_table_(symbol_table_),
    lookup_countries(std::move(lookup_countries)),
    root(root)
{
}

void codesh::semantic_analyzer::semantic_context::throw_blasphemy(std::string details,
                                                                  const lexer::code_range &source_range)
    const
{
    blasphemy_consumer(std::move(details), source_range);
}

void codesh::semantic_analyzer::semantic_context::throw_blasphemy(std::string details,
                                                                  const lexer::code_position code_pos) const
{
    blasphemy_consumer(std::move(details), {code_pos, code_pos});
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    blasphemy::blasphemy_consumer blasphemy_consumer) const
{
    return {symbol_table_, lookup_countries, root, std::move(blasphemy_consumer)};
}

codesh::semantic_analyzer::semantic_context codesh::semantic_analyzer::semantic_context::with_consumer(
    const std::string &wrapper_name, const std::string &object_name) const
{
    return with_consumer(blasphemy::create_nested_consumer(blasphemy_consumer, wrapper_name, object_name));
}
