#include "blasphemy_consumer.h"

#include "blasphemy_collector.h"

void codesh::blasphemy::semantic_consumer(std::string details)
{
    get_blasphemy_collector().add_blasphemy(std::move(details), blasphemy_type::SEMANTIC);
}