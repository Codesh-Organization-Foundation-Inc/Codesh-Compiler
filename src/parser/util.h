#pragma once

#include <memory>
#include <queue>

#include "token/token.h"
#include "parser/ast/var_reference/value_ast_node.h"

#include <optional>

namespace codesh
{
namespace definition
{
class fully_qualified_name;
}
namespace ast::type
{
class type_ast_node;
}
enum class token_group;
}


namespace codesh::parser::util
{

/**
 * Checks whether the group of the first token matches the requested one.
 * If so, pops it from the queue.
 * @return Whether the token group matches the requested
 */
bool consuming_check(std::queue<std::unique_ptr<token>> &tokens, token_group token_group,
        std::optional<std::reference_wrapper<std::unique_ptr<token>>> token_out = std::nullopt);

/**
 * Checks whether the group of the first token matches the requested one.
 * @return Whether the token group matches the requested
 */
bool peeking_check(const std::queue<std::unique_ptr<token>> &tokens, token_group token_group);

/**
 * Ensures the provided tokens list is not empty.
 * If not, raises a parsing error.
 */
void ensure_tokens_exist(const std::queue<std::unique_ptr<token>> &tokens,
                         const std::optional<std::string> &no_tokens_blasphemy_details = std::nullopt);

/**
 * Parses a Fully Qualified Class Name
 */
void parse_fqn(std::queue<std::unique_ptr<token>> &tokens, definition::fully_qualified_name &fqn_out);

/**
 * Ensures a colon exists at the current token, and consumes it.
 */
void ensure_end_op(std::queue<std::unique_ptr<token>> &tokens);

/**
 * Pops the latest token from the queue and returns it, transferring its ownership to the caller.
 * @return The consumed token
 */
[[nodiscard]] std::unique_ptr<token> consume_token(std::queue<std::unique_ptr<token>> &tokens,
    const std::optional<std::string> &no_tokens_blasphemy_details = std::nullopt);

/**
 * Pops the latest token from the queue and returns it, transferring its ownership to the caller.
 * If the token is not an identifier, throws.
 * @return The consumed token
 */
[[nodiscard]] std::unique_ptr<identifier_token> consume_identifier_token(std::queue<std::unique_ptr<token>> &tokens);

/**
 * Pops the latest token from the queue and returns it, transferring its ownership to the caller.
 * If the token is not an identifier OR a literal, throws.
 * @return The consumed token
 */
[[nodiscard]] std::unique_ptr<identifier_token> consume_alnum_identifier_token(
        std::queue<std::unique_ptr<token>> &tokens,
        const std::optional<std::string>& no_tokens_blasphemy_details = std::nullopt);

[[nodiscard]] std::unique_ptr<ast::type::type_ast_node> parse_type(std::queue<std::unique_ptr<token>> &tokens);

/**
 * Returns the display name of a token: its content if it's an identifier, or its keyword name otherwise.
 */
[[nodiscard]] std::string get_token_display_name(const token &token);

/**
 * Consumes the "by" operator token if present, or reports an error.
 * @return Whether the "by" token was consumed
 */
bool consume_by(std::queue<std::unique_ptr<token>> &tokens);

bool consume_punc_equal(std::queue<std::unique_ptr<token>> &tokens);

/**
 * Optionally consumes a leading `this` token (and its following dot), prepends "this" to @p fqn_out,
 * then delegates to parse_fqn() for the rest of the name.
 * Use this wherever both plain identifiers and `this.field` / `this.method` syntax should be accepted.
 */
void parse_this_and_fqn(std::queue<std::unique_ptr<token>> &tokens, definition::fully_qualified_name &fqn_out);

}
