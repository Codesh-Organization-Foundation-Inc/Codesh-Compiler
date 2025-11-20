#include "analyzer.h"

#include "passes/check_types.h"
#include "passes/verify_methods.h"

namespace codesh::semantic_analyzer
{
void run(ast::compilation_unit_ast_node& root)
{
    // build_symbol_table(root);
    check_types(root);
    check_methods(root);
    checkParameters(root);
}
}
