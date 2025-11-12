#pragma once
#include <vector>

namespace codesh::output::jvm_target::defs
{

struct cp_info
{
    unsigned char tag[1];

    virtual ~cp_info();
};

// tag = 1
struct CONSTANT_Utf8_info : cp_info
{
    unsigned char length[1];
    std::vector<unsigned char> bytes;
};

// tag = 3
struct CONSTANT_Integer_info : cp_info
{
    unsigned char bytes[4];
};

// tag = 4
struct CONSTANT_Float_info : cp_info
{
    unsigned char bytes[4];
};

// tag = 5
struct CONSTANT_Long_info : cp_info
{
    unsigned char high_bytes[4];
    unsigned char low_bytes[4];
};

// tag = 6
struct CONSTANT_Double_info : cp_info
{
    unsigned char high_bytes[4];
    unsigned char low_bytes[4];
};

// tag = 7
struct CONSTANT_Class_info : cp_info
{
    unsigned char name_index[2];
};

// tag = 8
struct CONSTANT_String_info : cp_info
{
    unsigned char string_index[2];
};

// tag = 9
struct CONSTANT_Fieldref_info : cp_info
{
    unsigned char class_index[2];
    unsigned char name_and_type_index[2];
};

// tag = 10
struct CONSTANT_Methodref_info : cp_info
{
    unsigned char class_index[2];
    unsigned char name_and_type_index[2];
};

// tag = 11
struct CONSTANT_InterfaceMethodref_info : cp_info
{
    unsigned char class_index[2];
    unsigned char name_and_type_index[2];
};

// tag = 12
struct CONSTANT_NameAndType_info : cp_info
{
    unsigned char name_index[2];
    unsigned char descriptor_index[2];
};

// tag = 15
// struct CONSTANT_MethodHandle_info : cp_info
// {
//     unsigned char reference_kind[1];
//     unsigned char reference_index[2];
// };


}