#pragma once

#include <string>

namespace codesh::lexer::nikkud
{

/**
 * @returns Whether @p c is Nikkud or Ta'am
 */
[[nodiscard]] bool is_nikkud(char16_t c);

/**
 * @returns The position of the first non-Nikkud character at or before @p pos,
 * searching backwards in @p code
 */
[[nodiscard]] size_t skip_nikkud_backwards(const std::u16string &code, size_t pos);

/**
 * Builds a nikkud-stripped copy of @p code from @p code_pos into @p cleaned and
 * sets @p match_begin / @p match_end_ptr to its bounds.
 */
void create_nikkudless_match_params(const std::u16string &code, size_t code_pos,
        std::u16string &cleaned, const char16_t *&match_begin, const char16_t *&match_end_ptr);

/**
 * Maps a match length in the cleaned (nikkud-stripped) slice back to the corresponding
 * end offset in the original code, relative to @p code_pos.
 */
[[nodiscard]] size_t get_original_length(const std::u16string &code, size_t code_pos, size_t cleaned_length);

}
