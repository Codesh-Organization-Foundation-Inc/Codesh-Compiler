#include "command_parser.h"
#include "lexer/regex.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/build.h"
#include "output/jvm_target/defs/class_file.h"
#include "parser/parser.h"
#include "test.h"

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


    //TESTS
    //codesh::test::descriptor();
    //


    // LEXING
    // Convert the string to UTF-8.
    // Necessary because the compiler tokenizes non-ASCII characters (Hebrew and Maqaf)
    const std::u16string utf16_code = utf8::utf8to16(amen_file);
    auto tokens = codesh::lexer::tokenize_code(utf16_code);

    // PARSING
    const auto ast = codesh::parser::parse(tokens);

    // BUILDING
    const codesh::output::jvm_target::defs::class_file class_file = codesh::output::jvm_target::build(ast.get());
    codesh::output::jvm_target::write_to_file(class_file, std::string(args.dest_path));

    return 0;
}

std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open " + file_name);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}