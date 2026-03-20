#pragma once

#include <filesystem>
#include <string>
#include <libzippp.h>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::semantic_analyzer::external
{

[[nodiscard]] bool is_jar(const std::filesystem::path &path);

class jar_loader final
{
    libzippp::ZipArchive archive;

public:
    explicit jar_loader(const std::filesystem::path &path);
    [[nodiscard]] bool load(const std::string &class_candidate, const symbol_table &table) const;
};

}
