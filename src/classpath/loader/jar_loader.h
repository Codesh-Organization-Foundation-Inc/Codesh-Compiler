#pragma once

#include "class_loader.h"

#include <filesystem>
#include <libzippp.h>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::external
{

[[nodiscard]] bool is_jar(const std::filesystem::path &path);

class jar_loader final : public class_loader
{
    libzippp::ZipArchive archive;

public:
    explicit jar_loader(const std::filesystem::path &path);

    [[nodiscard]] bool load(const semantic_analyzer::symbol_table &table,
                            const definition::fully_qualified_name &class_name) override;
};

}
