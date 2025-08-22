#include "lexer/regex.h"
#include "token/token.h"

#include <iostream>

int main(const int argc, char **const argv) {
    // const codesh::command_args args = codesh::parse_command(argc, argv);

    const codesh::token *token_test = codesh::token::from_group_name("KEYWORD_TRY", "abcdef");
    const codesh::token *token_num = codesh::token::from_group_name("IDENTIFIER_NUMBER", "23423");

    std::cout << token_num->get_type() << std::endl;
    std::cout << codesh::LEXER_RGX << std::endl;

    delete token_test;
    delete token_num;

    return 0;
}
