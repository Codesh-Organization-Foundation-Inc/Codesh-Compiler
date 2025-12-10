#pragma once

#include <string>
#include <vector>

namespace codesh::output
{

struct local_variable_table_entry
{
    size_t start;
    size_t length;
    std::string name;
    std::string signature;
};

class local_variable_table
{
    std::vector<local_variable_table_entry> entries;

public:
    [[nodiscard]] const std::vector<local_variable_table_entry> &get_entries() const;

    void add_entry(local_variable_table_entry entry);
    [[nodiscard]] const local_variable_table_entry &get(size_t index) const;
};

}
