#pragma once

#include <filesystem>

namespace codesh::definition
{
class fully_qualified_name;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::semantic_analyzer::external
{

enum class jimage_location_attribute : unsigned char
{
    END,
    MODULE,
    PARENT,
    BASE,
    EXTENSION,
    OFFSET,
    COMPRESSED,
    UNCOMPRESSED,
    COUNT
};

/**
 * Loads all symbols of an external class file to the provided @p table
 * @returns Whether the requested classfile was found within the jimage
 */
bool load_jimage_class(const std::filesystem::path &path, const std::string &module_name,
        const definition::fully_qualified_name &class_name, const symbol_table &table);
}
