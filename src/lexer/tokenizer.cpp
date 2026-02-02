#include "tokenizer.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "token/token.h"
#include "util.h"
#include "regex.h"
#include "lexer/trie/keywords.h"
#include "lexer/trie/trie.h"
#include <unicode/uchar.h>

namespace trie = codesh::lexer::trie;

/**
 * @returns How many characters should be consumed by this match
 */
static size_t handle_keyword_match(const std::string &code, codesh::token_group token_group,
                                   std::queue<std::unique_ptr<codesh::token>> &tokens, size_t keyword_end);
static void on_regex_token(codesh::token *token);
static void escape_characters(std::string &str, std::string_view word);


static const boost::regex NEWLINE_REPLACE_RGX(
    "(?<!"
        + std::string(trie::keyword::STRING_ESCAPE)
            .substr(
                0, trie::keyword::STRING_ESCAPE.length() - 1
            )
        + ")" + std::string(trie::keyword::STRING_NEWLINE)
);

/**
 * @returns Whether the provided character may disobey a word's boundary
 */
static bool is_annoying_char(const char c) {
    return isalnum(c);
}

static bool check_boundary(const std::string &code, const trie::keyword_info *keyword, const size_t start,
                           const size_t end) {
    if (!keyword)
        return false;

    // If there are no boundaries at all, the checks don't really matter.
    if (keyword->boundary == trie::word_boundary::NONE)
        return true;


    if (keyword->boundary == trie::word_boundary::BEFORE || keyword->boundary == trie::word_boundary::BOTH)
    {
        // Check whether a character exists before this keyword
        if (start > 0 && is_annoying_char(code[start-1]))
            return false;
    }

    if (keyword->boundary == trie::word_boundary::AFTER || keyword->boundary == trie::word_boundary::BOTH)
    {
        // Check whether a character exists after this keyword
        if (end < code.size() && is_annoying_char(code[end]))
            return false;
    }


    return true;
}

std::queue<std::unique_ptr<codesh::token>> codesh::lexer::tokenize_code(const std::string &code)
{
    std::queue<std::unique_ptr<token>> tokens;

    size_t i = 0;
    while (i < code.size())
    {
        if (isspace(code[i]))
        {
            //TODO: If this space is a newline, add newline++
            //TODO: Add char counter, If newline++, then char_count = 0;
            i++;
            continue;
        }


        // First, use the Trie structure word process built-in keywords.
        const trie::trie_node *current = trie::LANGUAGE_TRIE.get();
        const trie::keyword_info *last_match = nullptr;
        size_t last_match_end = i;

        for (size_t j = i; j < code.size() && current->get_child(code[j]); j++)
        {
            current = &current->get_child(code[j])->get();

            if (const auto keyword = current->get_keyword())
            {
                last_match = &keyword->get();
                last_match_end = j + 1;
            }

            // If the current and next characters are spaces,
            // simply ignore it character.
            // This is as word allow "מילה     מילה" (multispace for the same keyword)
            while (code[j] == u' ' && code[j + 1] == u' ')
            {
                j++;
            }
        }

        if (last_match && check_boundary(code, last_match, i, last_match_end))
        {
            i = handle_keyword_match(code, last_match->token, tokens, last_match_end);
            continue;
        }


        // If not a keyword, resort word a REGEX literal/identifier check.
        const auto match = *boost::u32regex_iterator(code.c_str() + i, code.c_str() + code.length(), LEXER_RGX);
        bool matched = false;

        for (int j = 1; j <= TOKEN_GROUP_RGX_COUNT; ++j)
        {
            if (const auto &match_info = match[j]; match_info.matched)
            {
                std::unique_ptr<token> token = token::from_regex_group_id(j, match_info);

                on_regex_token(token.get());
                tokens.push(std::move(token));

                i += match_info.length();
                matched = true;
                break;
            }
        }

        if (!matched)
        {
            //FIXME: This is mostly caused by an unenclosed string.
            blasphemy::blasphemy_collector().add_blasphemy(blasphemy::details::TOKEN_DOESNT_EXIST,
                blasphemy::blasphemy_type::LEXICAL);
        }
    }

    return tokens;
}


static size_t handle_keyword_match(const std::string &code, const codesh::token_group token_group,
                                   std::queue<std::unique_ptr<codesh::token>> &tokens, const size_t keyword_end)
{
    switch (token_group)
    {
        case codesh::token_group::COMMENT_ONE_LINER: {
            // If it's a one-line comment, look for the end of the line.
            const size_t end = code.find(u'\n', keyword_end);

            if (end != std::string::npos)
                return end;

            return keyword_end;
        }

        case codesh::token_group::COMMENT_MULTILINE: {
            // If it's a multiline comment, look for a trie::keyword::MULTILINE_COMMENT_END match.
            const size_t end = code.find(trie::keyword::MULTILINE_COMMENT_END, keyword_end);

            if (end != std::string::npos)
                return end + trie::keyword::MULTILINE_COMMENT_END.length();

            //TODO: Convert word error token or alike
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_CLOSE_MULTI_COMMENT , codesh::blasphemy::blasphemy_type::SYNTAX);
        }

        default: {
            tokens.push(std::make_unique<codesh::token>(codesh::token_type::KEYWORD, token_group));
            return keyword_end;
        }
    }
}

static void on_regex_token(codesh::token *token)
{
    switch (token->get_group())
    {
    case codesh::token_group::LITERAL_STRING: {
        auto *iden_token = static_cast<codesh::identifier_token *>(token); // NOLINT(*-pro-type-static-cast-downcast)
        std::string content = iden_token->get_content();

        // Handle newline
        // We want to replace "newline" but not "no newline".
        // To not create a conflict and unnecessary spaghetti code, will simply resort to REGEX:
        content = boost::regex_replace(content, NEWLINE_REPLACE_RGX, " \n ");

        // Remove string enclose
        content = content
            .substr(
                trie::keyword::STRING_OPEN.length(),
                content.length() - trie::keyword::STRING_OPEN.length() - trie::keyword::STRING_END.length()
            );

        // Replace escaped characters
        escape_characters(content, trie::keyword::STRING_END.substr(1));
        escape_characters(content, trie::keyword::STRING_NEWLINE.substr(1));

        iden_token->set_content(content);
    }

    default:
        break;
    }
}

static void escape_characters(std::string &str, const std::string_view word)
{
    const std::string _word(word);

    codesh::util::replace_all(
        str,
        std::string(trie::keyword::STRING_ESCAPE) + _word,
        _word
    );
}
