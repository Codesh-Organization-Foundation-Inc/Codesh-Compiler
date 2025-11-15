#pragma once
#include <memory>
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

    [[nodiscard]] virtual size_t hash_code() const = 0;

    unsigned char tag;
};


class CONSTANT_Utf8_info : public cp_info
{
public:
    CONSTANT_Utf8_info();

    bool operator==(const CONSTANT_Utf8_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char length[2]{};
    std::vector<unsigned char> bytes;
};

class CONSTANT_Integer_info : public cp_info
{
public:
    CONSTANT_Integer_info();

    bool operator==(const CONSTANT_Integer_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char bytes[4]{};
};

class CONSTANT_Float_info : public cp_info
{
public:
    CONSTANT_Float_info();

    bool operator==(const CONSTANT_Float_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char bytes[4]{};
};

class CONSTANT_Long_info : public cp_info
{
public:
    CONSTANT_Long_info();

    bool operator==(const CONSTANT_Long_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Double_info : public cp_info
{
public:
    CONSTANT_Double_info();

    bool operator==(const CONSTANT_Double_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Class_info : public cp_info
{
public:
    CONSTANT_Class_info();

    bool operator==(const CONSTANT_Class_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char name_index[2]{};
};

class CONSTANT_String_info : public cp_info
{
public:
    CONSTANT_String_info();

    bool operator==(const CONSTANT_String_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char string_index[2]{};
};

class CONSTANT_Fieldref_info : public cp_info
{
public:
    CONSTANT_Fieldref_info();

    bool operator==(const CONSTANT_Fieldref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_Methodref_info : public cp_info
{
public:
    CONSTANT_Methodref_info();

    bool operator==(const CONSTANT_Methodref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_InterfaceMethodref_info : public cp_info
{
public:
    CONSTANT_InterfaceMethodref_info();

    bool operator==(const CONSTANT_InterfaceMethodref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_NameAndType_info : public cp_info
{
public:
    CONSTANT_NameAndType_info();

    bool operator==(const CONSTANT_NameAndType_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char name_index[2]{};
    unsigned char descriptor_index[2]{};
};


struct cp_info_ptr_hash
{
    size_t operator()(const cp_info *obj) const;
};
struct cp_info_ptr_equal
{
    bool operator()(const cp_info *lhs, const cp_info *rhs) const;
};

struct cp_info_unique_ptr_hash
{
    size_t operator()(const std::unique_ptr<const cp_info> &obj) const;
};
struct cp_info_unique_ptr_equal
{
    bool operator()(const std::unique_ptr<const cp_info> &lhs, const std::unique_ptr<const cp_info> &rhs) const;
};

}
