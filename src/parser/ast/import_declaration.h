#pragma once

#include <string>

namespace codesh::ast
{

struct import_declaration
{
    const std::string package_name;
    const bool is_on_demand;
    const bool is_static;
};

}
