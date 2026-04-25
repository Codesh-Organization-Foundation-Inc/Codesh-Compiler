#pragma once

namespace codesh::semantic_analyzer
{
struct semantic_context;

void verify_country_matches_source_path(const semantic_context &context);
}
