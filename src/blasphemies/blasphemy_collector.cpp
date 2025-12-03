#include "blasphemy_collector.h"

#include <iostream>

namespace codesh
{

void blasphemy_collector::add_blasphemy(std::string msg)
{
    blasphemies.emplace_back(std::move(msg));
}

void blasphemy_collector::add_blasphemy(std::string msg, code_position code_pos)
{
    blasphemies.emplace_back(std::move(msg), code_pos);
}

bool blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void blasphemy_collector::print_all_errors() const
{
    for (const auto &[message, code_pos] : blasphemies)
    {
        std::cerr << "Error";

        if (code_pos.has_value())
        {
            std::cerr
                << "at line " << code_pos->line
                << ", column " << code_pos->column;
        }

        std::cerr << ": " << message;
    }
}

}