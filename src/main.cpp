#include "lexer/regex.h"
#include "token/token_type.h"

#include <iostream>

int main() {
    std::cout << codesh::token::get_token_type("KEYWORD_TRY") << std::endl;
    std::cout << codesh::LEXER_RGX << std::endl;
    return 0;
}

// https://www.geeksforgeeks.org/c/regular-expressions-in-c/

