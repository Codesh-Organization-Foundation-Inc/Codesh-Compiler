#include "lexer/regex.h"
#include "lexer/tokenizer.h"

#include <iostream>
#include <queue>

int main(const int argc, char **const argv) {
    // const codesh::command_args args = codesh::parse_command(argc, argv);

    std::cout << codesh::LEXER_RGX_STR << std::endl;

    const auto tokens = codesh::tokenize_code("ויהי שלם ושמו א, נגלה יהיה, ויאמר:");

    return 0;
}
