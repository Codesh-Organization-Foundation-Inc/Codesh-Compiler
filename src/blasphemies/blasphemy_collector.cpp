#include "blasphemy_collector.h"

#include <iostream>
#include <utility>

namespace codesh::error
{

void blasphemy_collector::add_blasphemy(std::string msg)
{
    add_blasphemy(std::move(msg), blasphemy_type::UNKNOWN);
}

void blasphemy_collector::add_blasphemy(std::string msg, blasphemy_type type)
{
    blasphemies.emplace_back(std::move(msg), type);
}

void blasphemy_collector::add_blasphemy(std::string msg, blasphemy_type type, code_position code_pos)
{
    blasphemies.emplace_back(std::move(msg), type, code_pos);
}

bool blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void blasphemy_collector::print_all_errors() const
{
    for (const auto &[message,type, code_pos] : blasphemies)
    {
        std::cerr << "Error ";
        std::cerr << "of type: " << type_to_string(type);

        if (code_pos.has_value())
        {
            std::cerr
                << "at line " << code_pos->line
                << ", column " << code_pos->column;
        }

        std::cerr << ": " << message;
    }
}
std::string blasphemy_collector::type_to_string(const blasphemy_type type)
{
    switch (type)
    {
    case blasphemy_type::LEXICAL:
        return "Lexical";
    case blasphemy_type::SEMANTIC:
        return "Semantic";
    case blasphemy_type::SYNTAX:
        return "Syntax";
    case blasphemy_type::UNKNOWN:
        return "Unknown";
    }

    throw std::runtime_error("Unknown type");
}

blasphemy_collector &get_blasphemy_collector()
{
    static blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}

} // namespace codesh::error