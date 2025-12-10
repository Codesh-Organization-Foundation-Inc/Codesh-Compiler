#include "local_variable_table.h"

const std::vector<codesh::output::local_variable_table_entry> &codesh::output::local_variable_table::get_entries() const
{
    return entries;
}

void codesh::output::local_variable_table::add_entry(local_variable_table_entry entry)
{
    entries.emplace_back(std::move(entry));
}

const codesh::output::local_variable_table_entry &codesh::output::local_variable_table::get(const size_t index)
    const
{
    return entries.at(index);
}