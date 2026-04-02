#pragma once

#include "defenition/definitions.h"
#include "defenition/fully_qualified_name.h"

#include <filesystem>
#include <optional>
#include <vector>

namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::output::jvm_target
{

struct jar_builder_context
{
    const std::filesystem::path &jre_path;

    const std::filesystem::path &temp_class_dir;
    const std::filesystem::path &dest_jar_path;

    // Manifest-related
    const std::optional<definition::fully_qualified_name> &explicit_main_class;
    const definition::class_loaders &class_loaders;

    bool fat_jar = false;
};

/**
 * Packages all .class files in @c temp_class_dir into a JAR at @c dest_jar_path.
 * @returns Whether the operation succeeded
 */
[[nodiscard]] bool bundle_jar(const semantic_analyzer::symbol_table &symbol_table,
        const jar_builder_context &context);

}
