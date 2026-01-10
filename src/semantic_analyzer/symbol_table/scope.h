#pragma once

#include "semantic_analyzer/symbol_table/symbol_type.h"

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace codesh::semantic_analyzer
{
class symbol;

class symbols_collection
{
protected:
    const std::vector<symbol_type> allowed_symbol_types;

public:
    explicit symbols_collection(std::vector<symbol_type> allowed_symbol_types);
    virtual ~symbols_collection();

    [[nodiscard]] virtual bool is_empty() const = 0;
};

class named_symbol_map final : public symbols_collection
{
    std::unordered_map<std::string, std::unique_ptr<symbol>> symbols;

public:
    explicit named_symbol_map(std::vector<symbol_type> allowed_symbol_types);

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve_local(const std::string &name) const;
    [[nodiscard]] std::unique_ptr<symbol> resolve_and_move(const std::string &name);

    [[nodiscard]] bool is_empty() const override;
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<symbol>> &internals() const;


    template <std::derived_from<symbol> T>
    std::pair<std::reference_wrapper<T>, bool> add_symbol(std::string name, std::unique_ptr<T> entry);

    void remove_symbol(const std::string &name);
};

class symbol_list final : public symbols_collection
{
    std::vector<std::unique_ptr<symbol>> symbols;

public:
    explicit symbol_list(std::vector<symbol_type> allowed_symbol_types);

    [[nodiscard]] std::optional<std::reference_wrapper<symbol>> resolve(size_t index) const;
    [[nodiscard]] std::unique_ptr<symbol> resolve_and_move(size_t index);

    [[nodiscard]] bool is_empty() const override;


    template <std::derived_from<symbol> T>
    /**
     * @returns The same symbol and the index it was assigned to within the list.
     */
    std::pair<std::reference_wrapper<T>, size_t> add_symbol(std::unique_ptr<T> entry);
    void remove_symbol(size_t index);
};

}

#include "semantic_analyzer/symbol_table/scope.tpp"
