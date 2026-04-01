#pragma once

#include <optional>
#include "defenition/fully_qualified_name.h"

namespace codesh::semantic_analyzer::external
{

[[nodiscard]] std::optional<definition::fully_qualified_name> find_codesh_replacement(
        const definition::fully_qualified_name &name);

}
