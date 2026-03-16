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

static std::unique_ptr<codesh::lsp::request> process_lsp_request(const nlohmann::json &request);
static std::string get_file_uri(const nlohmann::json &params_obj);
static nlohmann::json build_response(const nlohmann::json &request);
static void send_response(const nlohmann::json &response);
static void write_to_dummy(const std::string &contents);

codesh::lsp::request::~request() = default;

codesh::lsp::diagnostics_request::diagnostics_request(std::string file_uri, std::string file_contents) :
    file_uri(std::move(file_uri)),
    file_contents(std::move(file_contents))
{
}

std::unique_ptr<codesh::lsp::request> codesh::lsp::wait_for_request()
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

    write_to_dummy(body);

    return process_lsp_request(nlohmann::json::parse(body));
}

static std::unique_ptr<codesh::lsp::request> process_lsp_request(const nlohmann::json &request)
{
    const auto method = request.at("method").get<std::string>();

    if (method == "initialize")
    {
        auto response = build_response(request);

        response["result"] = {
            {"capabilities", {
                {"textDocumentSync", 1}
            }}
        };

        send_response(response);
    }
    else if (method == "textDocument/didOpen")
    {
        const auto &params = request.at("params");


        return std::make_unique<codesh::lsp::diagnostics_request>(
            get_file_uri(params),
            params.at("textDocument").at("text").get<std::string>()
        );
    }
    else if (method == "textDocument/didChange")
    {
        const auto &params = request.at("params");

        return std::make_unique<codesh::lsp::diagnostics_request>(
            get_file_uri(params),
            params.at("contentChanges").at("text").get<std::string>()
        );
    }

    return nullptr;
}

static std::string get_file_uri(const nlohmann::json &params_obj)
{
    return params_obj.at("textDocument").at("uri").get<std::string>();
}

static nlohmann::json build_response(const nlohmann::json &request)
{
    return {
        {"id", request.at("id").get<int>()},
        {"jsonrpc", request.at("jsonrpc").get<std::string>()}
    };
}

static void send_response(const nlohmann::json &response)
{
    const std::string payload = response.dump();
    std::cout << CONTENT_LENGTH_KEY << payload.size() << "\r\n\r\n" << payload;
    std::cout.flush();

    write_to_dummy(payload);
}

static void write_to_dummy(const std::string &contents)
{
    std::ofstream temp(DUMMY_FILE, std::ios::app);
    temp << contents << "\n";
}
