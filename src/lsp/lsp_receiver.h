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
};

/**
 * @returns The LSP request. May be @c nullptr if the request was already handled by this function.
 */
std::unique_ptr<request> wait_for_request();

}
