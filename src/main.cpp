#include "lexer/regex.h"
#include "lexer/tokenizer.h"
#include "parser/command_parser.h"
#include "parser/parser.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <utf8.h>

std::string read_file(const std::string &file_name);

int main(const int argc, char **const argv) {
    const codesh::command_args args = codesh::parse_command(argc, argv);

    const std::string amen_file = read_file(std::string(args.src_path));

    // Convert the string to UTF-8.
    // Necessary because the compiler tokenizes non-ASCII characters (Hebrew and Maqaf)
    const std::u16string utf16_code = utf8::utf8to16(amen_file);
    auto tokens = codesh::lexer::tokenize_code(utf16_code);

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