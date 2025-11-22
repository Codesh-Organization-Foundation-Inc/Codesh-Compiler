//TODO

#pragma once

#include "../../output/jvm_target/class_file_builder.h"

#include <boost/container_hash/hash.hpp>
#include <list>
#include <memory>
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

class entry;

enum class symbol_kind
{
    TYPE,
    VARIABLE,
    FIELD,
    METHOD
};

class scope
{
    // int id;
    //
    // std::unordered_map<std::string, entry> entries;
    // std::unordered_map<int, std::unique_ptr<scope>> scopes;
    //
    // std::optional<std::reference_wrapper<scope>> *parent_scope;
    //TODO
};


class entry
{
    // symbol_kind symbol;
    // std::list<output::jvm_target::access_flag> access_flags;
    //
    // virtual ~entry();

    //TODO
};

class method_entry : public entry
{
    //TODO
};

}
