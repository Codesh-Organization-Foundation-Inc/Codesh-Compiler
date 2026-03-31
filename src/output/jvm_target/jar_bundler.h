#pragma once

#include <filesystem>

namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::output::jvm_target
{

/**
 * Packages all .class files in @c temp_class_dir into a JAR at @c dest_jar_path.
 * @returns Whether the operation succeeded
 */
[[nodiscard]] bool bundle_jar(const semantic_analyzer::symbol_table &symbol_table,
        const std::filesystem::path &temp_class_dir, const std::filesystem::path &dest_jar_path,
        const std::filesystem::path &jre_path);

}
