#include "interop_replacements.h"
#include "lexer/source_file_info.h"

#include <unordered_map>

static codesh::definition::fully_qualified_name make_fqn(const std::string& name);

static const std::unordered_map<
    codesh::definition::fully_qualified_name, codesh::definition::fully_qualified_name,
    codesh::definition::fully_qualified_name_hasher
> CODESH_REPLACEMENTS = {
    {make_fqn("System/out/println"), make_fqn("מסוף/אמר")},
    {make_fqn("System/out/print"), make_fqn("מסוף/הגד")},
    {make_fqn("Math/random"), make_fqn("מתמט/הגרל")},
    {make_fqn("Math/min"), make_fqn("מתמט/סיני")},
    {make_fqn("Math/max"), make_fqn("מתמט/ים־המלחים")},
    {make_fqn("Math/abs"), make_fqn("מתמט/ויחליט")}
};

static codesh::definition::fully_qualified_name make_fqn(const std::string& name)
{
    return codesh::definition::fully_qualified_name::parse(name, codesh::lexer::NO_CODE_POS);
}

std::optional<codesh::definition::fully_qualified_name> codesh::semantic_analyzer::external::find_codesh_replacement(
        const definition::fully_qualified_name &name)
{
    const auto it = CODESH_REPLACEMENTS.find(name);
    if (it == CODESH_REPLACEMENTS.end())
        return std::nullopt;

    return it->second;
}
