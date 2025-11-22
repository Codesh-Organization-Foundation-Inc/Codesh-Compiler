#pragma once
#include <algorithm>

template <std::derived_from<codesh::semantic_analyzer::symbol> T>
std::pair<std::reference_wrapper<T>, bool> codesh::semantic_analyzer::i_scope_containing_symbol::add_symbol(
        std::string name, std::unique_ptr<T> entry)
{
    const bool is_legal_symbol = std::ranges::any_of(
        allowed_symbol_types(),
        [&entry](const symbol_type type) {
            return type == entry->get_symbol_type();
        }
    );

    if (!is_legal_symbol)
        throw std::runtime_error("Illegal symbol entry");


    const auto [it, inserted] = get_symbol_map().emplace(std::move(name), std::move(entry));

    return {*static_cast<T *>(it->second.get()), inserted};
}
