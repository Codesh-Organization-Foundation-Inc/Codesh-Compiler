#include "lexer/regex.h"
#include "token/token.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

std::string read_file(const std::string &file_name);

int main(const int argc, char **const argv) {
    // const codesh::command_args args = codesh::parse_command(argc, argv);

    const codesh::token *token_test = codesh::token::from_group_id(10, "abcdef");
    const codesh::token *token_num = codesh::token::from_group_id(74, "23423");

    std::cout << codesh::LEXER_RGX << std::endl;

    delete token_test;
    delete token_num;

    std::cout << read_file("../rashi.amen") << std::endl;
    return 0;
}

std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);
    if (!file.is_open())
    {
        std::cerr << "Error: could not open file" << std::endl;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}