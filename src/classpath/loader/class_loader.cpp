#include "class_loader.h"

codesh::external::class_loader::class_loader() : _is_loaded(true)
{
}

codesh::external::class_loader::~class_loader() = default;

bool codesh::external::class_loader::is_loaded() const
{
    return _is_loaded;
}