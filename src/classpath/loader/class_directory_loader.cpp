#include "class_directory_loader.h"

#include "class_file_loader.h"
#include "defenition/fully_qualified_name.h"

codesh::external::class_directory_loader::class_directory_loader(std::filesystem::path directory)
    : path(std::move(directory))
{
}

bool codesh::external::class_directory_loader::load(const semantic_analyzer::symbol_table &symbol_table,
                                  const definition::fully_qualified_name &class_name)
{
    // A classpath directory may only contain class files
    const auto class_file_path = path / (class_name.get_last_part() + ".class");
    return class_file_loader(class_file_path).load(symbol_table);
}
