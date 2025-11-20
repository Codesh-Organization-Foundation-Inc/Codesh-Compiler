#include "analyzer.h"

#include "passes/check_types.h"

namespace codesh::semantic_analyzer
{
void run(ast::compilation_unit_ast_node& root)
{
    // build_symbol_table(root);
    checkTypes(root);
    checkMethods(root);
    checkParameters(root);
}
}
