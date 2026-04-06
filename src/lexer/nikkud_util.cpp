#include "nikkud_util.h"

/**
 * Appends characters from @p code starting at @p code_pos into @p out, skipping any nikkud.
 */
static void strip_nikkud(const std::u16string &code, size_t code_pos, std::u16string &out);


bool codesh::lexer::nikkud::is_nikkud(const char16_t c)
{
    return c >= u'\u0591' && c <= u'\u05C7'
        // Allow Maqaf Elion:
        && c != u'־';
}

size_t codesh::lexer::nikkud::skip_nikkud_backwards(const std::u16string &code, size_t pos)
{
    while (pos > 0 && is_nikkud(code[pos - 1]))
    {
        pos--;
    }

    return pos;
}

static void strip_nikkud(const std::u16string &code, const size_t code_pos, std::u16string &out)
{
    for (size_t i = code_pos; i < code.size(); i++)
    {
        if (!codesh::lexer::nikkud::is_nikkud(code[i]))
        {
            out += code[i];
        }
    }
}

void codesh::lexer::nikkud::create_match_params(const std::u16string &code, const size_t code_pos,
        std::u16string &cleaned, const char16_t *&match_begin, const char16_t *&match_end_ptr,
        const bool process_nikkud)
{
    if (!process_nikkud)
    {
        match_begin = code.c_str() + code_pos;
        match_end_ptr = code.c_str() + code.size();
        return;
    }

    strip_nikkud(code, code_pos, cleaned);
    match_begin = cleaned.c_str();
    match_end_ptr = cleaned.c_str() + cleaned.size();
}

size_t codesh::lexer::nikkud::get_original_length(const std::u16string &code, const size_t code_pos,
        const size_t cleaned_length)
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