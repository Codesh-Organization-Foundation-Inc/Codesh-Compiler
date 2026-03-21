#pragma once

#include "class_file_container_loader.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace codesh::definition
{
class fully_qualified_name;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::external
{

enum class jimage_location_attribute : unsigned char
{
    END,
    MODULE,
    PARENT,
    BASE,
    EXTENSION,
    OFFSET,
    COMPRESSED,
    UNCOMPRESSED,
    COUNT
};

struct jimage_offsets
{
    uint32_t table_length;
    uint32_t locations_size;
    uint32_t strings_size;
    std::streamoff offsets_start;
    std::streamoff locations_start;
    std::streamoff strings_start;
    std::streamoff data_start;
};

struct class_file_lookup_result
{
    /**
     * Bytes past data_start
     */
    uint64_t data_offset;
    /**
     * Contains a valid value when @c jimage_location_attribute::COMPRESSED: 0 otherwise
     */
    uint64_t compressed_size;
    /**
     * File size IF UNCOMPRESSED
     */
    uint64_t size;
};

[[nodiscard]] bool is_jimage(const std::filesystem::path &path);

class jimage_loader final : public class_file_container_loader
{
    static constexpr std::streamoff HEADER_SIZE = 28;

    std::ifstream _file;
    jimage_offsets _layout{};
    std::vector<int32_t> _redirect_table;
    std::vector<uint32_t> _offsets;
    std::vector<unsigned char> _locations;
    std::vector<char> _strings;

    [[nodiscard]] jimage_offsets parse_header();
    [[nodiscard]] std::vector<int32_t> load_redirect_table();
    [[nodiscard]] std::vector<uint32_t> load_offsets();
    [[nodiscard]] std::vector<unsigned char> load_locations();
    [[nodiscard]] std::vector<char> load_strings();

    [[nodiscard]] std::optional<int32_t> get_location_offset_index(const std::string &path) const;
    [[nodiscard]] std::optional<class_file_lookup_result> lookup_class_file(
            const std::string &module_name, const std::string &target_class) const;

    void load_compressed_class_file(std::streamoff file_offset, const class_file_lookup_result &lookup,
            const semantic_analyzer::symbol_table &table);

public:
    explicit jimage_loader(const std::filesystem::path &path);

    bool load(const std::string &module_name, const definition::fully_qualified_name &class_name,
              const semantic_analyzer::symbol_table &table);
};

}
