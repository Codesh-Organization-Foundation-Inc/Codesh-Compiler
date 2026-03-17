#include "source_keyword_info.h"

#include <boost/functional/hash.hpp>

size_t codesh::lexer::code_position_hasher::operator()(const blasphemy::code_position &pos) const noexcept
{
    size_t seed = 0;
    boost::hash_combine(seed, pos.line);
    boost::hash_combine(seed, pos.column);
    return seed;
}

codesh::lexer::global_source_info_map &codesh::lexer::get_global_source_info_map()
{
    static global_source_info_map instance;
    return instance;
}
