#pragma once

#include "class_loader.h"

#include <filesystem>

namespace codesh::definition
{
class fully_qualified_name;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::external
{

class class_directory_loader final : public class_loader
{
    std::filesystem::path path;

public:
    explicit class_directory_loader(std::filesystem::path directory);

    [[nodiscard]] bool load(const semantic_analyzer::symbol_table &symbol_table,
                            const definition::fully_qualified_name &class_name) override;
};

}
