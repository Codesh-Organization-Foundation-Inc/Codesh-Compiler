#pragma once

#include <filesystem>
#include <istream>

namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::semantic_analyzer::external
{
/**
 * Loads all symbols of an external class file to the provided @p table
 */
void load_class_file(const std::filesystem::path &path, const symbol_table &table);

/**
 * Loads all symbols of an external class file to the provided @p table
 */
void load_class_file(std::istream &file, const symbol_table &table);
}
