#include "blasphemy_collector.h"

#include <iostream>

namespace codesh
{

void blasphemy_collector::add_blasphemy(std::string msg)
{
    blasphemies.emplace_back( std::move(msg));
}

void blasphemy_collector::add_blasphemy(blasphemy_type type, std::string msg)
{
    blasphemies.emplace_back(std::move(msg), type);
}

void blasphemy_collector::add_blasphemy(blasphemy_type type, std::string msg, code_position code_pos)
{
    blasphemies.emplace_back(std::move(msg),type ,code_pos);
}

bool blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void blasphemy_collector::print_all_errors() const
{
    for (const auto &[message,type, code_pos] : blasphemies)
    {
        std::cerr << "Error";

        if (type.has_value())
        {
            std::cerr << "of type: " << type_to_string(type.value());
        }

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
    case blasphemy_type::Lexical:
        return "Lexical";
    case blasphemy_type::Semantic:
        return "Semantic";
    case blasphemy_type::Syntax:
        return "Syntax";
    }
    return "Unknown";
}

}