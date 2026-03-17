#include "blasphemy/blasphemy_collector.h"
#include "lsp_receiver.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

// Specs from https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/

static const std::string CONTENT_LENGTH_KEY = "Content-Length: ";
static const std::string CONTENT_TYPE_KEY = "Content-Type: ";

static std::unique_ptr<codesh::lsp::request> process_lsp_request(const nlohmann::json &request);
static std::string get_file_uri(const nlohmann::json &params_obj);
static nlohmann::json build_response(const nlohmann::json &request);
static void send_response(const nlohmann::json &response);
static void write_to_dummy(const std::string &contents);
static nlohmann::json blasphemy_to_diagnostic(const codesh::blasphemy::blasphemy_info &info, int severity);
static codesh::lexer::code_position token_to_lsp_end(const codesh::lexer::code_position &pos_1based, const std::optional<size_t> &file_id);

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

static nlohmann::json blasphemy_to_diagnostic(const codesh::blasphemy::blasphemy_info &info, int severity)
{
    // We use 1-based lines/columns; LSP uses 0-based.
    const bool has_range = info.source_range.has_value()
        && info.source_range->start != codesh::lexer::NO_CODE_POS;

    const auto start_pos = has_range
        ? codesh::lexer::code_position {
            info.source_range->start.line - 1,
            info.source_range->start.column - 1
        }
        : codesh::lexer::code_position {0, 0};

    const auto end_pos = has_range
        ? token_to_lsp_end(info.source_range->end, info.file_id)
        : codesh::lexer::code_position{0, 0};

    const nlohmann::json range = {
        {"start", {
            {"line", start_pos.line},
            {"character", start_pos.column}
        }},
        {"end", {
            {"line", end_pos.line},
            {"character", end_pos.column}
        }}
    };

    return {
        {"range", range},
        {"severity", severity},
        {"message", info.details}
    };
}

static codesh::lexer::code_position token_to_lsp_end(const codesh::lexer::code_position &pos_1based,
        const std::optional<size_t> &file_id)
{
    codesh::lexer::code_position result = {pos_1based.line - 1, pos_1based.column - 1};

    size_t keyword_length = 3; // default
    if (file_id.has_value())
    {
        const auto &keyword_infos = codesh::lexer::get_global_source_info_map()
            .at(*file_id).keyword_infos;

        if (const auto it = keyword_infos.find(pos_1based); it != keyword_infos.end())
            keyword_length = it->second.length;
    }

    result.column += keyword_length;
    return result;
}

void codesh::lsp::send_diagnostics_response(const diagnostics_request &request)
{
    const auto &collector = blasphemy::get_blasphemy_collector();

    nlohmann::json diagnostics = nlohmann::json::array();

    for (const auto &blasphemy : collector.get_all_blasphemies())
    {
        diagnostics.push_back(blasphemy_to_diagnostic(blasphemy, 1)); // 1 = Error
    }

    for (const auto &warning : collector.get_all_warnings())
    {
        diagnostics.push_back(blasphemy_to_diagnostic(warning, 2)); // 2 = Warning
    }

    const nlohmann::json notification = {
        {"jsonrpc", "2.0"},
        {"method", "textDocument/publishDiagnostics"},
        {"params", {
            {"uri", request.file_uri},
            {"diagnostics", diagnostics}
        }}
    };

    send_response(notification);
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
            params.at("contentChanges").at(0).at("text").get<std::string>()
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
    // static const std::string DUMMY_FILE = "/home/stavlpc/CLionProjects/Codesh-Compiler/lsp_dump.json";

    // std::ofstream temp(DUMMY_FILE, std::ios::app);
    // temp << contents << "\n";
}
