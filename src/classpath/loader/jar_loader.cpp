#include "jar_loader.h"

#include "blasphemy/blasphemy_collector.h"
#include "class_file_loader.h"
#include "util.h"

#include <cassert>
#include <fstream>
#include <sstream>

using codesh::external::jar_loader;

// Magic bytes at the start of every local file header in a ZIP archive
static constexpr uint32_t ZIP_LOCAL_SIG = 0x04034b50;
// Magic bytes of the End of Central Directory record (one per archive, near EOF)
static constexpr uint32_t ZIP_EOCD_SIG = 0x06054b50;

jar_loader::jar_loader(const std::filesystem::path &path) : archive(path.string())
{
    if (archive.open(libzippp::ZipArchive::ReadOnly) != LIBZIPPP_OK)
    {
        _is_loaded = false;
    }
}

bool codesh::external::is_jar(const std::filesystem::path &path)
{
    if (path.extension() != ".jar")
        return false;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return false;

    const auto sig = util::read_u4_le(file);
    return sig == ZIP_LOCAL_SIG || sig == ZIP_EOCD_SIG;
}

bool jar_loader::load(const std::string &class_candidate, const semantic_analyzer::symbol_table &table) const
{
    assert(is_loaded());

    const auto entry = archive.getEntry(class_candidate + ".class");
    if (entry.isNull())
        return false;

    std::stringstream stream;
    entry.readContent(stream);
    load_class_file(stream, table);
    return true;
}
