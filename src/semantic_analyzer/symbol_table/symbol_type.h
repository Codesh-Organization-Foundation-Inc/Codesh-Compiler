#pragma once

namespace codesh::semantic_analyzer
{

enum class symbol_type
{
    COUNTRY,
    TYPE,
    LOCAL_VARIABLE,
    FIELD,

    METHOD_OVERLOADS,
    METHOD,
    METHOD_SCOPE
};

}
