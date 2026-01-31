#include "symbol.h"

#include "defenition/primitive_type.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"

#include <utility>

codesh::semantic_analyzer::symbol::symbol(i_scope_containing_symbol *const parent_symbol, const symbol_type symbol_type) :
    _symbol_type(symbol_type),
    parent_symbol(parent_symbol)
{
}

codesh::semantic_analyzer::symbol::~symbol() = default;

codesh::semantic_analyzer::symbol_type codesh::semantic_analyzer::symbol::get_symbol_type() const
{
    return _symbol_type;
}

codesh::semantic_analyzer::i_scope_containing_symbol *codesh::semantic_analyzer::symbol::get_parent_symbol() const
{
    return parent_symbol;
}

codesh::semantic_analyzer::i_scope_containing_symbol::~i_scope_containing_symbol() = default;

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> codesh::semantic_analyzer::
    i_scope_containing_symbol::resolve_up(const std::string &name) const
{
    if (const auto &named_symbol_container = dynamic_cast<const named_symbol_map *>(&get_scope()))
    {
        const auto &result = named_symbol_container->resolve_local(name);

        if (result.has_value())
            return result;
    }

    if (const auto &parent = dynamic_cast<const symbol &>(*this).get_parent_symbol())
    {
        return parent->resolve_up(name);
    }

    return std::nullopt;
}

const codesh::definition::fully_qualified_name &codesh::semantic_analyzer::country_symbol::get_full_name() const
{
    return full_name;
}

codesh::semantic_analyzer::country_symbol::country_symbol(definition::fully_qualified_name full_name,
                                                          i_scope_containing_symbol *const parent_symbol,
                                                          ast::impl::ast_node *producing_node)
    : symbol(parent_symbol, symbol_type::COUNTRY), full_name(std::move(full_name)), scope(ALLOWED_SYMBOL_TYPES)
{
    // TODO:
    //  if (producing_node != nullptr)
    //  {
    //      producing_node->set_resolved(*this);
    //  }
}

codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::country_symbol::get_scope()
{
    return scope;
}

const codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::country_symbol::get_scope() const
{
    return scope;
}

codesh::ast::type_decl::type_declaration_ast_node *codesh::semantic_analyzer::type_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::variable_symbol::variable_symbol(i_scope_containing_symbol *const parent_symbol,
                                                            const symbol_type _symbol_type,
                                                            std::unique_ptr<ast::type::type_ast_node> type) :
    symbol(parent_symbol, _symbol_type),
    type(std::move(type))
{
}

codesh::ast::type::type_ast_node *codesh::semantic_analyzer::variable_symbol::get_type() const
{
    return type.get();
}

codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::type_symbol::get_scope()
{
    return scope;
}

const codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::type_symbol::get_scope() const
{
    return scope;
}

codesh::semantic_analyzer::type_symbol::type_symbol(i_scope_containing_symbol *const parent_symbol,
        definition::fully_qualified_name full_name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        ast::type_decl::type_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::TYPE),
    full_name(std::move(full_name)),
    scope(ALLOWED_SYMBOL_TYPES),
    producing_node(producing_node),
    attributes(std::move(attributes))
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

const codesh::definition::fully_qualified_name &codesh::semantic_analyzer::type_symbol::get_full_name() const
{
    return full_name;
}

const codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::type_symbol::get_attributes() const
{
    return *attributes;
}

codesh::semantic_analyzer::field_symbol::field_symbol(i_scope_containing_symbol *const parent_symbol,
                                                      definition::fully_qualified_name full_name,
                                                      std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
                                                      std::unique_ptr<ast::type::type_ast_node> type,
                                                      ast::local_variable_declaration_ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::FIELD, std::move(type)),
    full_name(std::move(full_name)),
    attributes(std::move(attributes)),
    producing_node(producing_node)
{
}

codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::field_symbol::get_attributes() const
{
    return *attributes;
}

const codesh::definition::fully_qualified_name &codesh::semantic_analyzer::field_symbol::get_full_name() const
{
    return full_name;
}

codesh::ast::local_variable_declaration_ast_node *codesh::semantic_analyzer::field_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::local_variable_symbol::local_variable_symbol(i_scope_containing_symbol *const parent_symbol,
        std::unique_ptr<ast::type::type_ast_node> type, const size_t index,
        ast::local_variable_declaration_ast_node *producing_node) :
    variable_symbol(parent_symbol, symbol_type::LOCAL_VARIABLE, std::move(type)),
    producing_node(producing_node),
    index(index)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

codesh::ast::local_variable_declaration_ast_node *codesh::semantic_analyzer::local_variable_symbol::get_producing_node()
    const
{
    return producing_node;
}

size_t codesh::semantic_analyzer::local_variable_symbol::get_jvm_index() const
{
    return index;
}

codesh::semantic_analyzer::method_overloads_symbol::method_overloads_symbol(
    i_scope_containing_symbol *const parent_symbol)
    : symbol(parent_symbol, symbol_type::METHOD_OVERLOADS), scope(ALLOWED_SYMBOL_TYPES)
{
}

codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::method_overloads_symbol::get_scope()
{
    return scope;
}

const codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::method_overloads_symbol::get_scope()
    const
{
    return scope;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> codesh::semantic_analyzer::
    method_overloads_symbol::resolve_method(const std::string &params_descriptor) const
{
    const auto result = scope.resolve_local(params_descriptor);

    if (!result.has_value())
        return std::nullopt;

    return *static_cast<method_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}

codesh::semantic_analyzer::method_scope_symbol::method_scope_symbol(i_scope_containing_symbol *const parent_symbol,
        indexed_locals_container &index_to_local_variable,
        ast::method::method_scope_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD_SCOPE),
    producing_node(producing_node),
    index_to_local_variable(index_to_local_variable),
    scope(ALLOWED_SYMBOL_TYPES)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);
    }
}

codesh::ast::method::method_scope_ast_node *codesh::semantic_analyzer::method_scope_symbol::get_producing_node() const
{
    return producing_node;
}

size_t codesh::semantic_analyzer::method_scope_symbol::add_variable(const std::string &name,
                                                                    std::unique_ptr<local_variable_symbol> variable)
{
    const size_t index = variable->get_jvm_index();

    const auto result = scope.add_symbol(name, std::move(variable));
    index_to_local_variable.locals.emplace(name, result.first);

    return index;
}

codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::method_scope_symbol::get_scope()
{
    return scope;
}

const codesh::semantic_analyzer::named_symbol_map &codesh::semantic_analyzer::method_scope_symbol::get_scope() const
{
    return scope;
}

codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_scope_symbol::add_inner_scope(
        std::unique_ptr<method_scope_symbol> method_scope)
{
    return *inner_scopes.emplace_back(std::move(method_scope));
}

codesh::semantic_analyzer::symbols_collection &codesh::semantic_analyzer::method_symbol::get_scope()
{
    return scope;
}

codesh::semantic_analyzer::method_symbol::method_symbol(i_scope_containing_symbol *const parent_symbol, type_symbol &parent_type,
        definition::fully_qualified_name full_name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::vector<std::unique_ptr<ast::type::type_ast_node>> parameter_types,
        std::unique_ptr<ast::type::type_ast_node> return_type, ast::method::method_declaration_ast_node *producing_node) :
    symbol(parent_symbol, symbol_type::METHOD),
    full_name(std::move(full_name)),
    attributes(std::move(attributes)),
    parameter_types(std::move(parameter_types)),
    return_type(std::move(return_type)),
    scope(ALLOWED_SYMBOL_TYPES),
    method_scope(nullptr),
    producing_node(producing_node),
    parent_type(parent_type)
{
    if (producing_node != nullptr)
    {
        producing_node->set_resolved(*this);

        // Add the only method scope.
        // (A method declaration can have at most only one scope)
        method_scope = &scope.add_symbol(
            create_method_scope(*this, producing_node->get_method_scope())
        ).first.get();
    }

    // If an AST didn't produce this node, then it was external.
    // In which case, it doesn't have a scope (as in we don't care about it).
}

std::unique_ptr<codesh::semantic_analyzer::method_scope_symbol> codesh::semantic_analyzer::method_symbol::
    create_method_scope(i_scope_containing_symbol &parent_scope, ast::method::method_scope_ast_node &producing_node)
{
    return std::make_unique<method_scope_symbol>(&parent_scope, local_variables, &producing_node);
}

const codesh::definition::fully_qualified_name &codesh::semantic_analyzer::method_symbol::get_full_name() const
{
    return full_name;
}

void codesh::semantic_analyzer::method_symbol::set_full_name(definition::fully_qualified_name name)
{
    full_name = std::move(name);
}

const codesh::ast::type_decl::attributes_ast_node &codesh::semantic_analyzer::method_symbol::get_attributes() const
{
    return *attributes;
}

const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &codesh::semantic_analyzer::method_symbol::
    get_parameter_types() const
{
    return parameter_types;
}

codesh::ast::type::type_ast_node &codesh::semantic_analyzer::method_symbol::get_return_type() const
{
    return *return_type;
}

const codesh::semantic_analyzer::indexed_locals_container &codesh::semantic_analyzer::method_symbol::
    get_all_local_variables() const
{
    return local_variables;
}

codesh::semantic_analyzer::method_scope_symbol &codesh::semantic_analyzer::method_symbol::get_method_scope() const
{
    return *method_scope;
}

const codesh::semantic_analyzer::symbol_list &codesh::semantic_analyzer::method_symbol::get_scope() const
{
    return scope;
}

size_t codesh::semantic_analyzer::method_scope_symbol::add_variable(ast::local_variable_declaration_ast_node &variable)
{
    size_t jvm_index = index_to_local_variable.slots_used;
    // A variable gets 2 slots if is either a long or a double
    bool gets_extra_slot = false;

    if (const auto primitive_type = dynamic_cast<const ast::type::primitive_type_ast_node *>(variable.get_type()))
    {
        switch (primitive_type->get_type())
        {
        case definition::primitive_type::LONG:
        case definition::primitive_type::DOUBLE:
            gets_extra_slot = true;
            break;

        default:
            break;
        }
    }

    index_to_local_variable.slots_used += 1 + gets_extra_slot;

    return add_variable(variable.get_name(), std::make_unique<local_variable_symbol>(
        this,
        variable.get_type()->clone(),
        jvm_index,
        &variable
    ));
}

codesh::ast::method::method_declaration_ast_node *codesh::semantic_analyzer::method_symbol::get_producing_node() const
{
    return producing_node;
}

codesh::semantic_analyzer::type_symbol &codesh::semantic_analyzer::method_symbol::get_parent_type() const
{
    return parent_type;
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::country_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::TYPE,
    symbol_type::COUNTRY
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::type_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::METHOD_OVERLOADS,
    symbol_type::FIELD
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::method_overloads_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::METHOD,
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::method_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::METHOD_SCOPE,
};

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::method_scope_symbol::ALLOWED_SYMBOL_TYPES = {
    symbol_type::LOCAL_VARIABLE,
};
