#include "nikkud_util.h"

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

void codesh::lexer::nikkud::create_nikkudless_match_params(const std::u16string &code, const size_t code_pos,
        std::u16string &cleaned, const char16_t *&match_begin, const char16_t *&match_end_ptr)
{
    for (size_t i = code_pos; i < code.size(); i++)
    {
        if (!is_nikkud(code[i]))
        {
            cleaned += code[i];
        }
    }

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