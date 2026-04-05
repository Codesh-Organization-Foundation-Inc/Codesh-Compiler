#include "tokenizer.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "lexer/trie/keywords.h"
#include "lexer/trie/trie.h"
#include "regex.h"
#include "source_file_info.h"
#include "token/token.h"
#include "util.h"
#include <optional>
#include <unicode/uchar.h>
#include <utf8.h>

namespace trie = codesh::lexer::trie;

/**
 * Pushes a new file entry to @c get_global_source_info_map
 *
 * @returns The new file's associated ID to access @c get_global_source_info_map, and a pointer to the entry itself.
 */
static std::pair<size_t, codesh::lexer::source_file_info *> create_file_entry();

static void advance_position(codesh::lexer::code_position &pos, char16_t c);

/**
 * @returns Whether the code at @p code_pos, ignoring Nikkud, starts a string literal
 */
static bool starts_string(const std::u16string &code, size_t code_pos);

/**
 * @returns The position of the first non-Nikkud character at or before @p pos,
 * searching backwards in @p code
 */
static size_t skip_nikkud_backwards(const std::u16string &code, size_t pos);

static void step_keyword(size_t &code_pos, size_t new_code_pos, codesh::lexer::code_position &curr_keyword_pos,
        codesh::lexer::source_file_info &source_info, const std::u16string &code);

/**
 * @returns How many characters should be consumed by this match
 */
static size_t handle_keyword_match(const std::u16string &code, codesh::lexer::code_position current_code_position,
                                   codesh::token_group token_group,
                                   std::queue<std::unique_ptr<codesh::token>> &tokens, size_t keyword_end);

static std::optional<size_t> try_match_trie_keyword(const std::u16string &code,
                                                    codesh::lexer::code_position current_code_position,
                                                    std::queue<std::unique_ptr<codesh::token>> &tokens, size_t code_pos);

static std::optional<size_t> try_match_regex_token(const std::u16string &code,
                                                   codesh::lexer::code_position current_code_position,
                                                   std::queue<std::unique_ptr<codesh::token>> &tokens, size_t code_pos);

/**
 * Prepares the regex input range for @c try_match_regex_token.
 *
 * For string literals, the raw code is used directly (to preserve nikkud).
 * For all other tokens, nikkud is stripped in @c cleaned and it will be used.
 *
 * @returns @c true if the original code was preserved
 */
[[nodiscard]] static bool create_match_params(const std::u16string &code, size_t code_pos, std::u16string &cleaned,
                                              const char16_t *&match_begin, const char16_t *&match_end_ptr);

/**
 * Maps a match length in the cleaned (nikkud-stripped) slice back to the corresponding
 * end offset in the original code, relative to @p code_pos.
 */
[[nodiscard]] static size_t get_original_length(const std::u16string &code, size_t code_pos, size_t cleaned_length);

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
static bool is_annoying_char(const char16_t c)
{
    return u_isalnum(c);
}

/**
 * @returns Whether @p c is Nikkud or Ta'am
 */
static bool is_nikkud(const char16_t c)
{
    return c >= u'\u0591' && c <= u'\u05C7'
        // Allow Maqaf Elion:
        && c != u'־';
}

static bool check_boundary(const std::u16string &code, const trie::word_boundary boundary, const size_t start,
                           const size_t end) {
    // If there are no boundaries at all, the checks don't really matter.
    if (boundary == trie::word_boundary::NONE)
        return true;

    if (boundary == trie::word_boundary::BEFORE || boundary == trie::word_boundary::BOTH)
    {
        const auto before = skip_nikkud_backwards(code, start);
        if (before > 0 && is_annoying_char(code[before - 1]))
            return false;
    }

    if (boundary == trie::word_boundary::AFTER || boundary == trie::word_boundary::BOTH)
    {
        // Check whether a character exists after this keyword
        if (end < code.size() && is_annoying_char(code[end]))
            return false;
    }

    return true;
}

codesh::lexer::lexing_result codesh::lexer::tokenize_code(std::filesystem::path path, const std::string &code)
{
    // Convert the string to UTF-8.
    // Necessary because the compiler tokenizes non-ASCII characters (Hebrew and Maqaf)
    const std::u16string utf16_code = utf8::utf8to16(code);
    return tokenize_code(std::move(path), utf16_code);
}

codesh::lexer::lexing_result codesh::lexer::tokenize_code(std::filesystem::path path, const std::u16string &code)
{
    lexing_result result;
    auto &tokens = result.tokens;

    const auto [file_id, source_info] = create_file_entry();
    source_info->path = std::move(path);
    result.file_id = file_id;

    // main.cpp only provided the blasphemy collector with the file path.
    // Now it can also have access to the file ID.
    blasphemy::get_blasphemy_collector().set_source_file(file_id);


    code_position curr_keyword_pos = FILE_BEGIN;

    size_t code_pos = 0;
    while (code_pos < code.size())
    {
        advance_position(curr_keyword_pos, code[code_pos]);

        if (u_isspace(code[code_pos]))
        {
            code_pos++;
            continue;
        }

        // First, use the Trie structure word process built-in keywords.
        if (const auto new_code_pos = try_match_trie_keyword(code, curr_keyword_pos, tokens, code_pos))
        {
            step_keyword(code_pos, *new_code_pos, curr_keyword_pos, *source_info, code);
            continue;
        }

        // If not a keyword, resort to a REGEX literal/identifier check.
        if (const auto new_code_pos = try_match_regex_token(code, curr_keyword_pos, tokens, code_pos))
        {
            step_keyword(code_pos, *new_code_pos, curr_keyword_pos, *source_info, code);
            continue;
        }

        //FIXME: This is mostly caused by an unenclosed string.
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::TOKEN_DOESNT_EXIST,
            blasphemy::blasphemy_type::LEXICAL,
            curr_keyword_pos
        );
        code_pos++;
    }

    return result;
}

static void step_keyword(size_t &code_pos, const size_t new_code_pos, codesh::lexer::code_position &curr_keyword_pos,
        codesh::lexer::source_file_info &source_info, const std::u16string &code)
{
    const size_t keyword_length = new_code_pos - code_pos;

    source_info.keyword_infos.emplace(
        curr_keyword_pos,
        codesh::lexer::source_keyword_info {
            keyword_length
        }
    );

    for (size_t i = code_pos + 1; i < new_code_pos; i++)
    {
        advance_position(curr_keyword_pos, code[i]);
    }

    code_pos = new_code_pos;
}

static void advance_position(codesh::lexer::code_position &pos, const char16_t c)
{
    if (c == u'\n')
    {
        pos.line++;
        pos.column = 0;
    }
    else
    {
        pos.column++;
    }
}

static std::pair<size_t, codesh::lexer::source_file_info *> create_file_entry()
{
    auto &keyword_infos = codesh::lexer::get_global_source_info_map();

    const auto new_id = keyword_infos.size();
    auto &entry = keyword_infos.emplace_back();

    return {new_id, &entry};
}

static std::optional<size_t> try_match_trie_keyword(const std::u16string &code,
                                                    const codesh::lexer::code_position current_code_position,
                                                    std::queue<std::unique_ptr<codesh::token>> &tokens,
                                                    const size_t code_pos)
{
    const trie::trie_node *current = &trie::get_language_trie();
    std::optional<trie::trie_match> last_match;
    size_t last_match_end = code_pos;

    for (size_t i = code_pos; i < code.size(); i++)
    {
        if (is_nikkud(code[i]))
            continue;

        if (!current->get_child(code[i]))
            break;

        current = &current->get_child(code[i])->get();

        if (const auto keyword = current->get_match())
        {
            last_match = keyword;
            last_match_end = i + 1;
            // Advance past any nikkud/te'amim that immediately follow this match position
            while (last_match_end < code.size() && is_nikkud(code[last_match_end]))
                last_match_end++;
        }

        // If the current and next characters are spaces,
        // simply ignore it character.
        // This is as word allow "מילה     מילה" (multispace for the same keyword)
        while (code[i] == u' ' && i + 1 < code.size() && code[i + 1] == u' ')
        {
            i++;
        }
    }

    if (last_match && check_boundary(code, last_match->boundary, code_pos, last_match_end))
    {
        return handle_keyword_match(
            code,
            current_code_position,
            last_match->keyword_token,
            tokens,
            last_match_end
        );
    }

    return std::nullopt;
}

static std::optional<size_t> try_match_regex_token(const std::u16string &code,
                                                   const codesh::lexer::code_position current_code_position,
                                                   std::queue<std::unique_ptr<codesh::token>> &tokens,
                                                   const size_t code_pos)
{
    // For non-string tokens a nikkud-stripped slice is built and matched against.
    std::u16string cleaned;
    const char16_t *match_begin;
    const char16_t *match_end_ptr;

    const bool is_clean = create_match_params(
        code,
        code_pos,
        cleaned,
        match_begin,
        match_end_ptr
    );

    const auto match = *boost::utf16regex_iterator(match_begin, match_end_ptr, codesh::lexer::LEXER_RGX);

    for (int i = 1; i <= codesh::lexer::TOKEN_GROUP_RGX_COUNT; ++i)
    {
        if (const auto &match_info = match[i]; match_info.matched)
        {
            auto token = codesh::token::from_regex_group_id(
                current_code_position,
                i,
                match_info
            );

            on_regex_token(token.get());
            tokens.push(std::move(token));

            if (is_clean)
                return code_pos + match_info.length();

            // Map the cleaned match length back to the original code position
            return code_pos + get_original_length(code, code_pos, match_info.length());
        }
    }

    return std::nullopt;
}

static bool create_match_params(const std::u16string &code, const size_t code_pos, std::u16string &cleaned,
        const char16_t *&match_begin, const char16_t *&match_end_ptr)
{
    if (starts_string(code, code_pos))
    {
        // Keep nikkud
        match_begin = code.c_str() + code_pos;
        match_end_ptr = code.c_str() + code.length();
        return true;
    }

    // Skip all nikkud
    for (size_t i = code_pos; i < code.size(); i++)
    {
        if (!is_nikkud(code[i]))
        {
            cleaned += code[i];
        }
    }

    match_begin = cleaned.c_str();
    match_end_ptr = cleaned.c_str() + cleaned.size();
    return false;
}

static size_t get_original_length(const std::u16string &code, const size_t code_pos, const size_t cleaned_length)
{
    size_t count = 0;

    for (size_t i = code_pos; i < code.size(); i++)
    {
        if (count == cleaned_length)
            return i - code_pos;

        if (!is_nikkud(code[i]))
        {
            count++;
        }
    }

    return code.size() - code_pos;
}

static size_t handle_keyword_match(const std::u16string &code, codesh::lexer::code_position current_code_position,
                                   const codesh::token_group token_group,
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

        case codesh::token_group::COMMENT_MULTILINE_DOCS:
        case codesh::token_group::COMMENT_MULTILINE: {
            // If it's a multiline comment, look for a trie::keyword::MULTILINE_COMMENT_END match.
            const size_t end = code.find(trie::keyword::MULTILINE_COMMENT_END, keyword_end);

            if (end != std::string::npos)
                return end + trie::keyword::MULTILINE_COMMENT_END.length();

            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::NO_CLOSE_MULTI_COMMENT,
                codesh::blasphemy::blasphemy_type::SYNTAX,
                current_code_position
            );
        }

        default: {
            tokens.push(
                std::make_unique<codesh::token>(current_code_position, codesh::token_type::KEYWORD, token_group)
            );

            return keyword_end;
        }
    }
}

/**
 * Whether the string contains any character that foreign languages (like Java) will not like,
 * aka risking interoperability.
 */
static std::optional<char> contains_non_compliant_char(const std::string &content)
{
    const auto it = std::ranges::find_if(content, [](const char c) {
        return c == '"' || c == '\'';
    });

    return it != content.end()
        ? std::optional(*it)
        : std::nullopt;
}

static void on_regex_token(codesh::token *token)
{
    switch (token->get_group())
    {
    case codesh::token_group::LITERAL_NUMBER_INT:
    case codesh::token_group::LITERAL_NUMBER_FLOAT:
    case codesh::token_group::LITERAL_NUMBER_DOUBLE: {
        // Prefer using biblical numbers as a language convention
        codesh::blasphemy::get_blasphemy_collector().add_warning(
            fmt::format(
                codesh::blasphemy::details::NON_BIBLICAL_NUMBER,
                static_cast<const codesh::identifier_token &>(*token).get_content() // NOLINT(*-pro-type-static-cast-downcast)
            ),
            codesh::blasphemy::blasphemy_type::LEXICAL,
            token->get_code_position()
        );
        break;
    }

    case codesh::token_group::IDENTIFIER: {
        const auto *iden_token = static_cast<codesh::identifier_token *>(token); // NOLINT(*-pro-type-static-cast-downcast)

        if (const auto non_kosher_char = contains_non_compliant_char(iden_token->get_content()))
        {
            codesh::blasphemy::get_blasphemy_collector().add_warning(
                fmt::format(
                    codesh::blasphemy::details::ILLEGAL_IDENTIFIER_CONTENT_FOR_NON_KOSHER_LANGUAGES,
                    *non_kosher_char
                ),
                codesh::blasphemy::blasphemy_type::LEXICAL,
                iden_token->get_code_position()
            );
        }
        break;
    }

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
        break;
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

static bool starts_string(const std::u16string &code, const size_t code_pos)
{
    static const std::u16string STRING_OPEN_U16 = utf8::utf8to16(std::string(trie::keyword::STRING_OPEN));
    size_t j = 0;

    for (size_t i = code_pos; i < code.size() && j < STRING_OPEN_U16.size(); i++)
    {
        if (is_nikkud(code[i]))
            continue;
        if (code[i] != STRING_OPEN_U16[j])
            return false;

        j++;
    }

    return j == STRING_OPEN_U16.size();
}

static size_t skip_nikkud_backwards(const std::u16string &code, size_t pos)
{
    while (pos > 0 && is_nikkud(code[pos - 1]))
    {
        pos--;
    }

    return pos;
}
