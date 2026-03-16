#include "type_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/visibility.h"
#include "fmt/format.h"
#include "parser/ast/operator/assignment/assign_operator_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/ast/type_declaration/error_type_declaration_ast_node.h"
#include "parser/ast/type_declaration/variable_declaration_ast_node.h"
#include "parser/util.h"
#include "parser/value/value_parser.h"
#include "parser/type/class/class_parser.h"

namespace ast = codesh::ast;

static std::unique_ptr<ast::op::assignment::assign_operator_ast_node> create_init_var_assignment_node(
        const ast::type_decl::variable_declaration_ast_node &dest,
        codesh::blasphemy::code_position pos,
        codesh::blasphemy::code_position name_pos,
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::parser::var_decl_assignment_policy policy);


std::unique_ptr<ast::type_decl::type_declaration_ast_node> codesh::parser::parse_type_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    const auto declaration_pos = util::consume_token(tokens)->get_code_position();

    switch (util::consume_token(tokens, blasphemy::details::UNEXPECTED_DECLARATION)->get_group())
    {
    case token_group::KEYWORD_CLASS: return parse_class_declaration(declaration_pos, tokens);
    case token_group::KEYWORD_INTERFACE:; //TODO
    case token_group::KEYWORD_ENUM:; //TODO
    case token_group::KEYWORD_ANNOTATION: return nullptr; //TODO

    default:
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::UNEXPECTED_DECLARATION,
            blasphemy::blasphemy_type::SYNTAX,
            declaration_pos
        );
        return std::make_unique<ast::type_decl::error_type_declaration_ast_node>(declaration_pos);
    }
}


std::unique_ptr<ast::op::assignment::assign_operator_ast_node> codesh::parser::parse_variable_declaration(
        ast::type_decl::variable_declaration_ast_node &dest,
        const blasphemy::code_position pos,
        std::queue<std::unique_ptr<token>> &tokens,
        const var_decl_assignment_policy policy)
{
    dest.set_type(util::parse_type(tokens));

    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX, pos
        );
    }

    const auto name_token = util::consume_identifier_token(tokens);
    dest.set_name(name_token->get_content());
    dest.set_attributes(parse_modifiers(pos, tokens));

    auto assignment = create_init_var_assignment_node(
        dest,
        pos,
        name_token->get_code_position(),
        tokens,
        policy
    );

    util::ensure_end_op(tokens);
    return assignment;
}

static std::unique_ptr<ast::op::assignment::assign_operator_ast_node> create_init_var_assignment_node(
        const ast::type_decl::variable_declaration_ast_node &dest,
        const codesh::blasphemy::code_position pos,
        const codesh::blasphemy::code_position name_pos,
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const codesh::parser::var_decl_assignment_policy policy)
{
    std::unique_ptr<codesh::token> assignment_token;
    const bool has_val_assignment = codesh::parser::util::consuming_check(
        tokens,
        codesh::token_group::KEYWORD_LET,
        assignment_token
    );

    if (policy == codesh::parser::var_decl_assignment_policy::REQUIRE && !has_val_assignment)
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_LET,
            codesh::blasphemy::blasphemy_type::SYNTAX, pos
        );
    }

    if (policy == codesh::parser::var_decl_assignment_policy::FORBID)
    {
        if (has_val_assignment)
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    "{}: {}",
                    codesh::blasphemy::details::UNEXPECTED_TOKEN,
                    codesh::parser::util::get_token_display_name(*assignment_token)
                ),
                codesh::blasphemy::blasphemy_type::SYNTAX,
                assignment_token->get_code_position()
            );
        }
        return nullptr;
    }

    if (!has_val_assignment)
        return nullptr;

    return std::make_unique<ast::op::assignment::assign_operator_ast_node>(
        assignment_token->get_code_position(),
        std::make_unique<variable_reference_ast_node>(
            pos,
            codesh::definition::fully_qualified_name(name_pos, dest.get_name())
        ),
        codesh::parser::value::parse_value(tokens)
    );
}


std::unique_ptr<ast::type_decl::attributes_ast_node> codesh::parser::parse_modifiers(
        blasphemy::code_position code_position, std::queue<std::unique_ptr<token>> &tokens)
{
    auto node = std::make_unique<ast::type_decl::attributes_ast_node>(
        code_position
    );

    // Attributes are optional, so check whether they exist at all.
    bool attributes_exist = false;


    // Optional 1: Static
    if (util::consuming_check(tokens, token_group::KEYWORD_STATIC))
    {
        node->set_is_static(true);
        attributes_exist = true;
    }

    // Optional 2: Visibility
    if (const auto visibility = definition::token_group_to_visibility(tokens.front().get()))
    {
        node->set_visibility(visibility.value());
        tokens.pop();
        attributes_exist = true;
    }

    // Optional 3: Abstract
    if (util::consuming_check(tokens, token_group::KEYWORD_ABSTRACT))
    {
        node->set_is_abstract(true);
        attributes_exist = true;
    }

    // Optional 4: Final
    if (util::consuming_check(tokens, token_group::KEYWORD_FINAL))
    {
        node->set_is_final(true);
        attributes_exist = true;
    }


    if (!attributes_exist)
        return node;

    // If the last keyword wasn't Shall Be, it means that the user entered a nonsensical keyword before,
    // or did not close the attribute statement with Shall Be.
    if (!util::consuming_check(tokens, token_group::KEYWORD_SHALL_BE))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_SHALL_BE,
            blasphemy::blasphemy_type::SYNTAX,
            code_position
        );
    }

    return node;
}