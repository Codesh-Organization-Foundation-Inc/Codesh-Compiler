#include "lexer/regex.h"
#include "lexer/tokenizer.h"
#include "parser/command_parser.h"
#include "parser/parser.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

std::string read_file(const std::string &file_name);

int main(const int argc, char **const argv) {
    const codesh::command_args args = codesh::parse_command(argc, argv);

    std::cout << codesh::LEXER_RGX_STR << std::endl;

    const std::string amen_file = read_file(std::string(args.src_path));
    auto tokens = codesh::lexer::tokenize_code(amen_file);

    const auto ast = codesh::parse(tokens);

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