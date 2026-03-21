#pragma once

#include <filesystem>

namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::external
{
/**
 * Loads all symbols of an external class file to the provided @p table
 */
void load_class_file(const std::filesystem::path &path, const semantic_analyzer::symbol_table &table);

/**
 * Loads all symbols of an external class file to the provided @p table
 */
void load_class_file(std::istream &file, const semantic_analyzer::symbol_table &table);
}
