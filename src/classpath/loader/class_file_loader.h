#pragma once

#include "class_loader.h"

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "defenition/fully_qualified_name.h"

namespace codesh::ast::type { class type_ast_node; }
namespace codesh::ast::type { class custom_type_ast_node; }
namespace codesh::ast::type_decl { class attributes_ast_node; }

namespace codesh::semantic_analyzer
{
class symbol_table;
class type_symbol;
}


namespace codesh::external
{
class class_file_loader final : public class_loader
{
    std::filesystem::path path;

    using cp_strings = std::unordered_map<int, std::string>;

    static void skip_attributes(std::istream &file, uint16_t count);
    static std::string get_utf8(const cp_strings &strings, int idx);
    static definition::fully_qualified_name get_class_name(const cp_strings &strings, int idx);
    static void parse_constant_pool(std::istream &file, cp_strings &strings);
    static std::vector<definition::fully_qualified_name> read_interface_names(std::istream &file,
            const cp_strings &strings);
    static void read_magic(std::istream &file);
    static semantic_analyzer::type_symbol &parse_type(std::istream &file, const cp_strings &strings,
            const semantic_analyzer::symbol_table &table);
    static void parse_fields(std::istream &file, const cp_strings &strings,
            semantic_analyzer::type_symbol &type_sym);
    static void parse_methods(std::istream &file, const cp_strings &strings,
            semantic_analyzer::type_symbol &type_sym);
    static std::unique_ptr<ast::type_decl::attributes_ast_node> flags_to_attributes(uint16_t flags);
    static std::unique_ptr<ast::type::type_ast_node> descriptor_to_node_type(const std::string &descriptor,
            size_t &pos);
    [[nodiscard]] static std::vector<std::unique_ptr<ast::type::custom_type_ast_node>>
            read_exceptions_attribute(std::istream &file, const cp_strings &strings);
    static void add_method_symbol(const std::string &method_descriptor, const std::string &method_name,
            std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
            std::vector<std::unique_ptr<ast::type::custom_type_ast_node>> sins_thrown,
            semantic_analyzer::type_symbol &type_sym);

public:
    explicit class_file_loader(std::filesystem::path path);

    [[nodiscard]] bool load(const semantic_analyzer::symbol_table &symbol_table) const;
    [[nodiscard]] bool load(const semantic_analyzer::symbol_table &symbol_table,
            const definition::fully_qualified_name &) override;

    /**
     * Loads all symbols of an external class file to the provided @p table
     */
    static void load(const semantic_analyzer::symbol_table &table, std::istream &file);
};

}
