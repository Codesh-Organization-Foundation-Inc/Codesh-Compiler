#include "blasphemy_consumer.h"

#include "blasphemy_collector.h"

codesh::blasphemy::blasphemy_consumer codesh::blasphemy::create_nested_consumer(
        const blasphemy_consumer &parent_consumer, const std::string &wrapper_name, const std::string &object_name)
{
    return [&parent_consumer, &wrapper_name, &object_name](std::string details) {
        parent_consumer(fmt::format(
            "{} {} {}",
            std::move(details),
            wrapper_name,
            object_name
        ));
    };
}

void codesh::blasphemy::semantic_consumer(std::string details)
{
    get_blasphemy_collector().add_blasphemy(std::move(details), blasphemy_type::SEMANTIC);
}