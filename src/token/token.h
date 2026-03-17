#pragma once

#include <string>

#include "blasphemy/blasphemy_collector.h"
#include "token_group.h"

#include <memory>

namespace codesh
{

enum class token_type : int;


class token
{
    const lexer::code_position code_position;

    const token_type type;
    const token_group group;

public:
    token(lexer::code_position code_position, token_type type, token_group group);
    virtual ~token();

    [[nodiscard]] static std::unique_ptr<token> from_regex_group_id(lexer::code_position code_position,
            int group_id, const std::u16string &content);


    [[nodiscard]] lexer::code_position get_code_position() const;

    [[nodiscard]] token_type get_type() const;
    [[nodiscard]] token_group get_group() const;

    static token_type get_token_type(token_group name);
};

class identifier_token final : public token
{
    std::string content;

public:
    identifier_token(lexer::code_position code_position, token_group group, std::string content);

    [[nodiscard]] std::string get_content() const;
    void set_content(std::string content);
};


enum class token_type : int
{
    KEYWORD,
    IDENTIFIER,
    // Doesn't really matter as this gets treated as a keyword, i.e no need to store content.
    //PUNCTUATION
};

}
