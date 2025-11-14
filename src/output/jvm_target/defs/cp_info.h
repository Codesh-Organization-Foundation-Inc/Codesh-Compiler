#pragma once
#include <vector>

namespace codesh::output::jvm_target::defs
{

//NOTE: All classes here SHOULD ACT like structs.
// This is defined as a class only for the convenience of auto-placing tags.

class cp_info
{
public:
    explicit cp_info(unsigned char tag);
    virtual ~cp_info();

    unsigned char tag;
};


class CONSTANT_Utf8_info : public cp_info
{
public:
    CONSTANT_Utf8_info();

    unsigned char length[2]{};
    std::vector<unsigned char> bytes;
};

class CONSTANT_Integer_info : public cp_info
{
public:
    CONSTANT_Integer_info();

    unsigned char bytes[4]{};
};

class CONSTANT_Float_info : public cp_info
{
public:
    CONSTANT_Float_info();

    unsigned char bytes[4]{};
};

class CONSTANT_Long_info : public cp_info
{
public:
    CONSTANT_Long_info();

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Double_info : public cp_info
{
public:
    CONSTANT_Double_info();

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Class_info : public cp_info
{
public:
    CONSTANT_Class_info();

    unsigned char name_index[2]{};
};

class CONSTANT_String_info : public cp_info
{
public:
    CONSTANT_String_info();

    unsigned char string_index[2]{};
};

class CONSTANT_Fieldref_info : public cp_info
{
public:
    CONSTANT_Fieldref_info();

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_Methodref_info : public cp_info
{
public:
    CONSTANT_Methodref_info();

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_InterfaceMethodref_info : public cp_info
{
public:
    CONSTANT_InterfaceMethodref_info();

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_NameAndType_info : public cp_info
{
public:
    CONSTANT_NameAndType_info();

    unsigned char name_index[2]{};
    unsigned char descriptor_index[2]{};
};

// class CONSTANT_MethodHandle_info : public cp_info
// {
// public:
//     CONSTANT_MethodHandle_info();
//
//     unsigned char reference_kind{};
//     unsigned char reference_index[2]{};
// };


}