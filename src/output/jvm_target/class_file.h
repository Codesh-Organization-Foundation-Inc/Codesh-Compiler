#pragma once

namespace codesh::output::jvm_target
{

struct class_file
{
    unsigned char magic[4];
    unsigned char minor_version[2];
    unsigned char major_version[2];
    unsigned char constant_pool_count[2];

};

}