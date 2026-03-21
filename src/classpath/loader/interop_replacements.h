#pragma once

#include <optional>
#include "defenition/fully_qualified_name.h"

namespace codesh::external
{

[[nodiscard]] std::optional<definition::fully_qualified_name> find_codesh_replacement(
        const definition::fully_qualified_name &name);

}
