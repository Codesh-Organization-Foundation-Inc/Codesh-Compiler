#pragma once

#include "../../output/jvm_target/class_file_builder.h"
#include "symbol_table_entry.h"

#include <boost/container_hash/hash.hpp>
#include <string>
#include <unordered_map>

namespace codesh::definition
{
enum class visibility;
}
namespace codesh::ast::impl
{
class ast_node;
}


namespace codesh::semantic_analyzer
{

class scope
{
    std::unordered_map<std::string, std::unique_ptr<entry>> entries;
    const std::optional<std::reference_wrapper<scope>> parent_scope;

public:
    scope();
    explicit scope(scope *parent_scope);

    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<entry>> &get_entries() const;
    [[nodiscard]] std::unordered_map<std::string, std::unique_ptr<entry>> get_entries();

    [[nodiscard]] const std::optional<std::reference_wrapper<scope>> &get_parent_scope() const;
};

}
