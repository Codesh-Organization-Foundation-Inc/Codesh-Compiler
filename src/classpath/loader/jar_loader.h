#pragma once

#include <filesystem>
#include <string>
#include <libzippp.h>

namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::external
{

[[nodiscard]] bool is_jar(const std::filesystem::path &path);

class jar_loader final
{
    libzippp::ZipArchive archive;
    bool _is_loaded;

public:
    explicit jar_loader(const std::filesystem::path &path);
    /**
     * @return Whether the requested JAR is loaded fine
     */
    [[nodiscard]] bool is_loaded() const;

    [[nodiscard]] bool load(const std::string &class_candidate, const semantic_analyzer::symbol_table &table) const;
};

}
