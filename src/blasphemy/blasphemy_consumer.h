#pragma once

#include <functional>
#include <string>

namespace codesh::blasphemy
{
using blasphemy_consumer = std::function<void(std::string details)>;

void semantic_consumer(std::string details);
}
