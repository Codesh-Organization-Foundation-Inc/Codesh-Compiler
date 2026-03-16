#pragma once

#include <memory>

namespace codesh::lsp
{

struct request
{
    virtual ~request();
};

struct diagnostics_request final : request
{
    std::string file_uri;
    std::string file_contents;

    diagnostics_request(std::string file_uri, std::string file_contents);
};

/**
 * @returns The LSP request. May be @c nullptr if the request was already handled by this function.
 */
[[nodiscard]] std::unique_ptr<request> wait_for_request();

void send_diagnostics_response(const diagnostics_request &request);

}
