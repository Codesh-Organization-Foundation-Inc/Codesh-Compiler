#pragma once

#include "attribute_info_entry.h"

namespace codesh::output::jvm_target::defs
{
struct fields_info_entry
{
    unsigned char access_flags[2];
    unsigned char name_index[2];
    unsigned char descriptor_index[2];
    unsigned char attributes_count[2];
    std::vector<std::unique_ptr<attribute_info_entry>> attribute_info;
};
}
