#pragma once

#include <memory>
#include <string>
#include <vector>

namespace codesh::output::jvm_target::defs
{

enum class constant_info_type : unsigned char
{
    UTF8 = 1,
    INTEGER = 3,
    FLOATING = 4,
    LONG_INT = 5,
    DOUBLE_FP = 6,
    CLASS_REF = 7,
    STRING_REF = 8,
    FIELDREF = 9,
    METHODREF = 10,
    INTERFACE_METHODREF = 11,
    NAME_AND_TYPE = 12,
};

//NOTE: All classes here SHOULD ACT like structs.
// This is defined as a class only for the convenience of auto-placing tags.

class cp_info
{
public:
    explicit cp_info(constant_info_type type);
    virtual ~cp_info();

    [[nodiscard]] virtual size_t hash_code() const = 0;

    unsigned char tag;
};


class CONSTANT_Utf8_info : public cp_info
{
public:
    explicit CONSTANT_Utf8_info(const std::string &utf8);

    bool operator==(const CONSTANT_Utf8_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char length[2]{};
    std::vector<unsigned char> bytes;
};

class CONSTANT_Integer_info : public cp_info
{
public:
    explicit CONSTANT_Integer_info(int num);

    bool operator==(const CONSTANT_Integer_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char bytes[4]{};
};

class CONSTANT_Float_info : public cp_info
{
public:
    explicit CONSTANT_Float_info(float num);

    bool operator==(const CONSTANT_Float_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char bytes[4]{};
};

class CONSTANT_Long_info : public cp_info
{
public:
    explicit CONSTANT_Long_info(long long num);

    bool operator==(const CONSTANT_Long_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Double_info : public cp_info
{
public:
    explicit CONSTANT_Double_info(double num);

    bool operator==(const CONSTANT_Double_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char high_bytes[4]{};
    unsigned char low_bytes[4]{};
};

class CONSTANT_Class_info : public cp_info
{
public:
    explicit CONSTANT_Class_info(int name_index);

    bool operator==(const CONSTANT_Class_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char name_index[2]{};
};

class CONSTANT_String_info : public cp_info
{
public:
    explicit CONSTANT_String_info(int utf8_index);

    bool operator==(const CONSTANT_String_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char string_index[2]{};
};

class CONSTANT_Fieldref_info : public cp_info
{
public:
    CONSTANT_Fieldref_info(int class_index, int name_and_type_index);

    bool operator==(const CONSTANT_Fieldref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_Methodref_info : public cp_info
{
public:
    CONSTANT_Methodref_info(int class_index, int name_and_type_index);

    bool operator==(const CONSTANT_Methodref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_InterfaceMethodref_info : public cp_info
{
public:
    CONSTANT_InterfaceMethodref_info(int class_index, int name_and_type_index);

    bool operator==(const CONSTANT_InterfaceMethodref_info &other) const;
    [[nodiscard]] size_t hash_code() const override;

    unsigned char class_index[2]{};
    unsigned char name_and_type_index[2]{};
};

class CONSTANT_NameAndType_info : public cp_info
{
public:
    CONSTANT_NameAndType_info(int name_index, int descriptor_index);

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
