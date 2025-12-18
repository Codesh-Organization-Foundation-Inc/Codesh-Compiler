#pragma once

#include <functional>
#include <string>

namespace codesh::blasphemy
{
using blasphemy_consumer = std::function<void(std::string details)>;

blasphemy_consumer create_nested_consumer(const blasphemy_consumer &parent_consumer,
        const std::string &wrapper_name, const std::string &object_name);

void semantic_consumer(std::string details);
}
