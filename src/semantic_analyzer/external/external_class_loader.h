#pragma once

#include <filesystem>

namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::semantic_analyzer
{
/**
 * Loads all symbols of an external class file to the provided @p table
 */
void load_external_class_file(const std::filesystem::path &path, symbol_table &table);
}
