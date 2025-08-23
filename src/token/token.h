#pragma once

#include <string>

#include "token_group.h"

namespace codesh
{

enum class token_type : int;


class token
{
    const token_type type;
    const token_group group;

    static token_type get_token_type(token_group name);

public:
    token(token_type type, token_group group);
    virtual ~token();

    [[nodiscard]] static token *from_group_id(int group_id, const std::string &content);

    [[nodiscard]] token_type get_type() const;
    [[nodiscard]] token_group get_group() const;
};

class identifier_token final : public token
{
    const std::string content;

public:
    identifier_token(token_type type, token_group group, std::string content);
};


enum class token_type : int
{
    KEYWORD,
    IDENTIFIER,
    // Doesn't really matter as this gets treated as a keyword, i.e no need to store content.
    //PUNCTUATION
};

}
