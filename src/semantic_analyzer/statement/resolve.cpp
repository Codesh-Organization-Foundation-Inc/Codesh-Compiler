#include "resolve.h"

#include "semantic_analyzer/statement/cast/resolve.h"
#include "semantic_analyzer/statement/method_call/resolve.h"
#include "semantic_analyzer/statement/variable_reference/resolve.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/primitive_type.h"
#include "fmt/xchar.h"
#include "parser/ast/collection/collection_ast_node.h"
#include "parser/ast/collection/range_ast_node.h"
#include "parser/ast/impl/binary_ast_node.h"
#include "parser/ast/impl/unary_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/method/operation/block/for_ast_node.h"
#include "parser/ast/method/operation/block/if_ast_node.h"
#include "parser/ast/method/operation/block/while_ast_node.h"
#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/method/operation/array_length_ast_node.h"
#include "parser/ast/method/operation/new_array_ast_node.h"
#include "parser/ast/method/operation/return_ast_node.h"
#include "parser/ast/operator/cast/cast_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/array_access_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/util.h"
#include "semantic_analyzer/util/widen_util.h"

using namespace codesh::semantic_analyzer;

static bool resolve_value(const semantic_context &context,
                          codesh::ast::var_reference::value_ast_node &val_node,
                          const method_symbol &containing_method,
                          const method_scope_symbol &scope);

static bool resolve_scope(const semantic_context &context,
                          const method_symbol &containing_method,
                          const codesh::ast::method::method_scope_ast_node &scope_node);

static bool resolve_conditioned_scope(const semantic_context &context,
                          const method_symbol &containing_method,
                          const method_scope_symbol &scope,
                          const codesh::ast::block::conditioned_scope_container &conditioned_scope);

static bool is_primitive_type(const codesh::ast::var_reference::value_ast_node &val_node,
                              codesh::definition::primitive_type type,
                              const std::string &blasphemy_details);

static bool is_condition_boolean(const codesh::ast::var_reference::value_ast_node &val_node);

static bool is_collection(const codesh::ast::var_reference::value_ast_node &val_node);

static bool resolve_array_operand(const semantic_context &context,
                                  codesh::ast::op::array_access_ast_node &arr_access);

static bool resolve_array_index(const semantic_context &context,
                                const codesh::ast::op::array_access_ast_node &arr_access);


bool statement::resolve(const semantic_context &context,
                        ast::method::operation::method_operation_ast_node &stmnt,
                        const method_symbol &containing_method,
                        const method_scope_symbol &scope)
{
    if (const auto method_call = dynamic_cast<ast::method::operation::method_call_ast_node *>(&stmnt))
    {
        return method_call::resolve(context, *method_call, containing_method, scope);
    }

    if (const auto var_ref = dynamic_cast<ast::var_reference::variable_reference_ast_node *>(&stmnt))
    {
        return variable_reference::resolve(context, *var_ref, scope);
    }

    if (const auto str = dynamic_cast<ast::var_reference::evaluable_ast_node<std::string> *>(&stmnt))
    {
        return util::resolve_type_node(context, *str->get_type());
    }


    //TODO: Move to separate resolvers
    if (const auto if_node = dynamic_cast<ast::block::if_ast_node *>(&stmnt))
    {
        bool all_succeed = true;

        // If
        all_succeed &= resolve_conditioned_scope(context, containing_method, scope, if_node->get_if_branch());

        // Else-if
        for (const auto &else_if_branch : if_node->get_else_if_branches())
        {
            all_succeed &= resolve_conditioned_scope(context, containing_method, scope, else_if_branch);
        }

        // Else
        if (if_node->get_else_branch().has_value())
        {
            all_succeed &= resolve_scope(context, containing_method, if_node->get_else_branch()->get());
        }

        return all_succeed;
    }


    if (const auto while_node = dynamic_cast<ast::block::while_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, while_node->get_condition(), containing_method, scope);
        all_succeed &= resolve_scope(context, containing_method, while_node->get_body_scope());
        all_succeed &= is_condition_boolean(while_node->get_condition());
        return all_succeed;
    }

    if (const auto range = dynamic_cast<ast::collection::range_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, range->get_from(), containing_method, scope);
        all_succeed &= resolve_value(context, range->get_to(), containing_method, scope);
        all_succeed &= resolve_value(context, range->get_skip(), containing_method, scope);
        return all_succeed;
    }

    if (const auto for_node = dynamic_cast<ast::block::for_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, for_node->get_collection(), containing_method, scope);
        all_succeed &= resolve_scope(context, containing_method, for_node->get_body_scope());
        all_succeed &= resolve_scope(context, containing_method, for_node->get_iterator_declaration_scope());
        all_succeed &= is_collection(for_node->get_collection());
        return all_succeed;
    }

    if (const auto return_node = dynamic_cast<ast::method::operation::return_ast_node *>(&stmnt))
    {
        const auto ret_val = return_node->get_return_value();
        if (ret_val == nullptr)
            return true;

        // Perform compatibility check only if we could resolve the return value
        // Otherwise it's a useless, guaranteed error.
        if (!resolve_value(context, *ret_val, containing_method, scope))
            return false;


        const auto &expected_return_type = containing_method.get_return_type();

        auto [are_types_compatible, return_value] = util::make_widening_cast_maybe(
            context,
            return_node->take_return_value(),
            expected_return_type
        );
        return_node->set_return_value(std::move(return_value));

        if (!are_types_compatible)
        {
            context.throw_blasphemy(
                fmt::format(
                    blasphemy::details::RETURN_TYPE_MISMATCH,
                    ret_val->get_type()->to_pretty_string(),
                    expected_return_type.to_pretty_string()
                ),
                return_node->get_code_position()
            );
            return false;
        }

        return true;
    }


    if (const auto arr_length = dynamic_cast<ast::method::operation::array_length_ast_node *>(&stmnt))
    {
        if (!resolve_value(context, arr_length->get_child(), containing_method, scope))
            return false;

        const auto *child_type = arr_length->get_child().get_type();
        if (child_type->get_array_dimensions() == 0)
        {
            context.throw_blasphemy(
                fmt::format(blasphemy::details::NOT_AN_ARRAY, child_type->to_pretty_string()),
                arr_length->get_child().get_code_position()
            );
            return false;
        }

        return true;
    }

    if (const auto unary_op = dynamic_cast<ast::impl::unary_ast_node *>(&stmnt))
    {
        if (!resolve_value(context, unary_op->get_child(), containing_method, scope))
            return false;

        if (!unary_op->is_value_valid(context))
        {
            context.throw_blasphemy(fmt::format(
                blasphemy::details::UNARY_TYPE_MISMATCH,
                unary_op->get_child().get_type()->to_pretty_string(),
                unary_op->to_pretty_string()
            ), unary_op->get_code_position());
            return false;
        }

        return true;
    }

    if (const auto binary_op = dynamic_cast<ast::impl::binary_ast_node *>(&stmnt))
    {
        bool all_succeed = true;
        all_succeed &= resolve_value(context, binary_op->get_left(), containing_method, scope);
        all_succeed &= resolve_value(context, binary_op->get_right(), containing_method, scope);

        // Do not perform value validation with an invalid variable reference
        if (all_succeed)
        {
            binary_op->apply_widening_conversions();

            if (!binary_op->is_value_valid(context))
            {
                context.throw_blasphemy(
                    fmt::format(
                        blasphemy::details::BINARY_TYPE_MISMATCH,
                        binary_op->get_left().get_type()->to_pretty_string(),
                        binary_op->get_right().get_type()->to_pretty_string(),
                        binary_op->to_pretty_string()
                    ),
                    {
                        binary_op->get_code_position(),
                        binary_op->get_right().get_code_position()
                    }
                );
                all_succeed = false;
            }
        }
        return all_succeed;
    }

    if (const auto arr_access = dynamic_cast<ast::op::array_access_ast_node *>(&stmnt))
    {
        bool all_succeed = true;

        if (all_succeed &= resolve_value(context, arr_access->get_array(), containing_method, scope))
        {
            all_succeed &= resolve_array_operand(context, *arr_access);
        }

        if (all_succeed &= resolve_value(context, arr_access->get_index(), containing_method, scope))
        {
            all_succeed &= resolve_array_index(context, *arr_access);
        }

        return all_succeed;
    }

    if (const auto cast = dynamic_cast<ast::op::assignment::cast_ast_node *>(&stmnt))
    {
        return cast::resolve(context, *cast, containing_method, scope);
    }

    if (const auto new_arr = dynamic_cast<ast::op::new_array_ast_node *>(&stmnt))
    {
        bool all_succeed = true;

        all_succeed &= util::resolve_type_node(context, new_arr->get_element_type());

        for (auto &dim : new_arr->get_dimensions())
        {
            if (!resolve_value(context, *dim, containing_method, scope))
            {
                all_succeed = false;
                continue;
            }

            all_succeed &= is_primitive_type(
                *dim,
                definition::primitive_type::INTEGER,
                blasphemy::details::ARRAY_INDEX_NOT_INTEGER
            );
        }

        return all_succeed;
    }

    // Probably doesn't need to be resolved.
    return true;
}


static bool resolve_value(const semantic_context &context,
                          codesh::ast::var_reference::value_ast_node &val_node,
                          const method_symbol &containing_method,
                          const method_scope_symbol &scope)
{
    if (const auto var_ref = dynamic_cast<codesh::ast::var_reference::variable_reference_ast_node *>(&val_node))
    {
        if (!statement::variable_reference::resolve(context, *var_ref, scope))
            return false;
    }
    else if (!statement::resolve(context, val_node, containing_method, scope))
    {
        return false;
    }

    return val_node.get_type() != nullptr;
}

static bool resolve_scope(const semantic_context &context,
                          const method_symbol &containing_method,
                          const codesh::ast::method::method_scope_ast_node &scope_node)
{
    bool all_succeed = true;

    for (const auto &statement : scope_node.get_body())
    {
        all_succeed &= statement::resolve(
            context,
            *statement,
            containing_method,
            scope_node.get_resolved()
        );
    }

    return all_succeed;
}

static bool resolve_conditioned_scope(const semantic_context &context,
                                      const method_symbol &containing_method,
                                      const method_scope_symbol &scope,
                                      const codesh::ast::block::conditioned_scope_container &conditioned_scope)
{
    bool all_succeed = true;
    all_succeed &= resolve_value(context, *conditioned_scope.condition, containing_method, scope);
    all_succeed &= resolve_scope(context, containing_method, conditioned_scope.scope);
    all_succeed &= is_condition_boolean(*conditioned_scope.condition);
    return all_succeed;
}

static bool is_primitive_type(const codesh::ast::var_reference::value_ast_node &val_node,
                              const codesh::definition::primitive_type type,
                              const std::string &blasphemy_details)
{
    if (const auto prim_type = dynamic_cast<codesh::ast::type::primitive_type_ast_node *>(val_node.get_type()))
    {
        if (prim_type->get_type() == type)
            return true;
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        blasphemy_details,
        codesh::blasphemy::blasphemy_type::SEMANTIC,
        val_node.get_code_position()
    );
    return false;
}

static bool is_condition_boolean(const codesh::ast::var_reference::value_ast_node &val_node)
{
    return is_primitive_type(
        val_node,
        codesh::definition::primitive_type::BOOLEAN,
        codesh::blasphemy::details::CONDITION_NOT_BOOLEAN
    );
}

static bool is_collection(const codesh::ast::var_reference::value_ast_node &val_node)
{
    if (dynamic_cast<const codesh::ast::collection::collection_ast_node *>(&val_node))
        return true;

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        codesh::blasphemy::details::ITERATOR_NOT_COLLECTION,
        codesh::blasphemy::blasphemy_type::SEMANTIC,
        val_node.get_code_position()
    );
    return false;
}

static bool resolve_array_operand(const semantic_context &context,
                                  codesh::ast::op::array_access_ast_node &arr_access)
{
    const auto *arr_type = arr_access.get_array().get_type();
    if (arr_type == nullptr)
        return false;

    if (arr_type->get_array_dimensions() == 0)
    {
        context.throw_blasphemy(
            fmt::format(
                codesh::blasphemy::details::NOT_AN_ARRAY,
                arr_type->to_pretty_string()
            ),
            arr_access.get_array().get_code_position()
        );
        return false;
    }

    auto elem_type = arr_type->clone();
    elem_type->set_array_dimensions(arr_type->get_array_dimensions() - 1);
    arr_access.set_element_type(std::move(elem_type));
    return true;
}

static bool resolve_array_index(const semantic_context &context,
                                const codesh::ast::op::array_access_ast_node &arr_access)
{
    const auto *idx_prim = dynamic_cast<const codesh::ast::type::primitive_type_ast_node *>(
        arr_access.get_index().get_type()
    );

    if (idx_prim == nullptr || idx_prim->get_type() != codesh::definition::primitive_type::INTEGER)
    {
        context.throw_blasphemy(
            codesh::blasphemy::details::ARRAY_INDEX_NOT_INTEGER,
            arr_access.get_index().get_code_position()
        );
        return false;
    }
    return true;
}
