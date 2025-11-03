#pragma once
#include "attribute_info.h"

namespace codesh::output::jvm_target
{
struct methods_info
{
    unsigned char access_flags[2];
    unsigned char name_index[2];
    unsigned char descriptor_index[2];
    unsigned char attributes_count[2];
    std::vector<attribute_info *> attribute_info;
};

}