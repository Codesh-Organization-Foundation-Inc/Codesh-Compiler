#include "lsp_receiver.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

// Specs from https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/

static const std::string DUMMY_FILE = "/home/stavlpc/CLionProjects/Codesh-Compiler/lsp_dump.json";

static const std::string CONTENT_LENGTH_KEY = "Content-Length: ";
static const std::string CONTENT_TYPE_KEY = "Content-Type: ";

static codesh::lsp::request process_lsp_request(const nlohmann::json &body);


codesh::lsp::request codesh::lsp::wait_lsp_request()
{
    int content_length = -1;
    std::string body;

    std::string content_type = "application/vscode-jsonrpc; charset=utf-8";

    // Read headers: lines separated by \r\n, header block ends with \r\n\r\n
    std::string line;
    while (true)
    {
        line.clear();

        int chara;
        while ((chara = std::cin.get()) != EOF)
        {
            if (chara == '\r')
            {
                const int next = std::cin.peek();
                if (next == '\n')
                    std::cin.get();

                break;
            }
            line += static_cast<char>(chara);
        }

        if (line.empty())
            break; // Blank line = end of headers


        if (line.rfind(CONTENT_LENGTH_KEY, 0) == 0)
        {
            content_length = std::stoi(line.substr(CONTENT_LENGTH_KEY.size()));
        }
        else if (line.rfind(CONTENT_TYPE_KEY, 0) == 0)
        {
            content_type = line.substr(CONTENT_TYPE_KEY.size());
        }
    }

    if (content_length < 0)
        throw std::runtime_error("LSP request missing Content-Length header");

    // idk if it's necessary
    (void)content_type;

    body.resize(content_length);
    std::cin.read(body.data(), content_length);

    std::ofstream temp(DUMMY_FILE, std::ios::app);
    temp << body << "\n";

    return process_lsp_request(nlohmann::json::parse(body));
}

static codesh::lsp::request process_lsp_request(const nlohmann::json &body)
{
    return {};
}
