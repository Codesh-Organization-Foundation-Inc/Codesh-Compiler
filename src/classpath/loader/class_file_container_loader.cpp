#include "class_file_container_loader.h"

codesh::external::class_file_container_loader::class_file_container_loader() : _is_loaded(false)
{
}

codesh::external::class_file_container_loader::~class_file_container_loader() = default;

bool codesh::external::class_file_container_loader::is_loaded() const
{
    return _is_loaded;
}