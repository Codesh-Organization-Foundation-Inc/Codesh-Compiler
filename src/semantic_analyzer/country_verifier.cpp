#include "country_verifier.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "semantic_context.h"

#include <fmt/format.h>
#include <filesystem>

static std::vector<std::string> path_to_parts(const std::filesystem::path &path);
static std::string holy_join_parts(const std::vector<std::string> &parts);
static void throw_country_mismatch(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::string> &expected_parts);

void codesh::semantic_analyzer::verify_country_matches_source_path(const semantic_context &context)
{
    const auto &source_dir = blasphemy::get_blasphemy_collector().get_source_directory();

    std::error_code error;
    const auto rel_source_path = std::filesystem::relative(context.root.get_source_path(), source_dir, error);
    if (error)
        return;

    const auto expected_parts = path_to_parts(rel_source_path.parent_path());
    if (expected_parts == context.root.get_package_name().get_parts())
        return;

    throw_country_mismatch(context, expected_parts);
}


static std::vector<std::string> path_to_parts(const std::filesystem::path &path)
{
    std::vector<std::string> parts;

    for (const auto &segment : path)
    {
        auto segment_str = segment.string();
        if (!segment_str.empty() && segment_str != ".")
        {
            parts.push_back(std::move(segment_str));
        }
    }

    return parts;
}

static std::string holy_join_parts(const std::vector<std::string> &parts)
{
    return fmt::format("{}",
        fmt::join(parts, " ל־")
    );
}

static void throw_country_mismatch(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::string> &expected_parts)
{
    const auto &package = context.root.get_package_name();
    const auto pos = context.root.get_code_position();

    context.throw_blasphemy(
        fmt::format(
            codesh::blasphemy::details::COUNTRY_PATH_MISMATCH,
            package.holy_join(),
            holy_join_parts(expected_parts)
        ),

        package.get_parts().empty()
            ? codesh::lexer::code_range{pos, pos} // For the method to match for code_range below
            : package.get_source_range()
    );
}
