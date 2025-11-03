#pragma once
#include "cp_info.h"
#include "methods_info.h"
#include "attribute_info.h"

namespace codesh::output::jvm_target
{

struct class_file
{
    unsigned char magic[4];
    unsigned char minor_version[2];
    unsigned char major_version[2];
    unsigned char constant_pool_count[2];
    std::vector<cp_info *> constant_pool;
    unsigned char access_flags[2];
    unsigned char this_class[2];
    unsigned char super_class[2];
    unsigned char interfaces_count[2];
    unsigned char interfaces_info[0]; // change later
    unsigned char fields_count[2];
    unsigned char fields_info[0]; // change later
    unsigned char methods_count[2];
    std::vector<methods_info *> methods_info;
    unsigned char attribute_count[2];
    std::vector<attribute_info *> attribute_info;

};


}