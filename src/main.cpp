#include "lexer/regex.h"
#include "parser/command_parser.h"
#include "token/token_type.h"

#include <iostream>

int main(const int argc, char **const argv) {
    // const codesh::command_args args = codesh::parse_command(argc, argv);

    std::cout << codesh::token::get_token_type("KEYWORD_TRY") << std::endl;
    std::cout << codesh::LEXER_RGX << std::endl;

    return 0;
}
