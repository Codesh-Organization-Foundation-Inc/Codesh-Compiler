#pragma once

#include <memory>
#include <vector>

namespace codesh::output::jvm_target::defs
{

class verification_type_info
{
    const unsigned char tag;

protected:
    explicit verification_type_info(unsigned char tag);

public:
    virtual ~verification_type_info();

    [[nodiscard]] unsigned char get_tag() const;
};


class top_variable_info final : public verification_type_info
{
public:
    top_variable_info();
};

class integer_variable_info final : verification_type_info
{
public:
    integer_variable_info();
};

class float_variable_info final : verification_type_info
{
public:
    float_variable_info();
};

class long_variable_info final : verification_type_info
{
public:
    long_variable_info();
};

class double_variable_info final : verification_type_info
{
public:
    double_variable_info();
};

class null_variable_info final : verification_type_info
{
public:
    null_variable_info();
};

class uninitialized_this_variable_info final : verification_type_info
{
public:
    uninitialized_this_variable_info();
};

class object_variable_info final : verification_type_info
{
public:
    object_variable_info();

    unsigned char cpool_index[2]{};
};

class uninitialized_variable_info final : verification_type_info
{
public:
    uninitialized_variable_info();

    unsigned char offset[2]{};
};


class stack_map_frame
{
    const unsigned char frame_type;

protected:
    explicit stack_map_frame(unsigned char frame_type);

public:
    virtual ~stack_map_frame();

    [[nodiscard]] unsigned char get_frame_type() const;
};


class same_frame final : public stack_map_frame
{
public:
    explicit same_frame(unsigned char offset_delta);
};

class same_locals_1_stack_item_frame final : public stack_map_frame
{
public:
    explicit same_locals_1_stack_item_frame(unsigned char offset_delta);

    std::unique_ptr<verification_type_info> stack;
};

class same_locals_1_stack_item_frame_extended final : public stack_map_frame
{
public:
    same_locals_1_stack_item_frame_extended();

    unsigned char offset_delta[2]{};
    std::unique_ptr<verification_type_info> stack;
};

class chop_frame final : public stack_map_frame
{
public:
    /**
     * @param k The amount to chop off
     */
    explicit chop_frame(unsigned char k);

    unsigned char offset_delta[2]{};
};

class same_frame_extended final : public stack_map_frame
{
public:
    same_frame_extended();

    unsigned char offset_delta[2]{};
};

class append_frame final : public stack_map_frame
{
public:
    /**
     * @param k The amount to append
     */
    explicit append_frame(unsigned char k);

    unsigned char offset_delta[2]{};
    std::vector<std::unique_ptr<verification_type_info>> locals;
};

class full_frame final : public stack_map_frame
{
public:
    full_frame();

    unsigned char offset_delta[2]{};
    unsigned char number_of_locals[2]{};
    std::vector<std::unique_ptr<verification_type_info>> locals;
    unsigned char number_of_stack_items[2]{};
    std::vector<std::unique_ptr<verification_type_info>> stack;
};

}
