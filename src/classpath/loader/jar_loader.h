#pragma once

#include "class_file_container_loader.h"

#include <filesystem>
#include <libzippp.h>
#include <string>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::external
{

[[nodiscard]] bool is_jar(const std::filesystem::path &path);

class jar_loader final : public class_file_container_loader
{
    libzippp::ZipArchive archive;

public:
    explicit jar_loader(const std::filesystem::path &path);

    [[nodiscard]] bool load(const std::string &class_candidate, const semantic_analyzer::symbol_table &table) const;
};

}
