#include "stack_map_table.h"

codesh::output::jvm_target::defs::verification_type_info::verification_type_info(const unsigned char tag) : tag(tag)
{
}

codesh::output::jvm_target::defs::verification_type_info::~verification_type_info() = default;

unsigned char codesh::output::jvm_target::defs::verification_type_info::get_tag() const
{
    return tag;
}

codesh::output::jvm_target::defs::top_variable_info::top_variable_info() :
    verification_type_info(0)
{}
codesh::output::jvm_target::defs::integer_variable_info::integer_variable_info() :
    verification_type_info(1)
{}
codesh::output::jvm_target::defs::float_variable_info::float_variable_info() :
    verification_type_info(2)
{}
codesh::output::jvm_target::defs::long_variable_info::long_variable_info() :
    verification_type_info(4)
{}
codesh::output::jvm_target::defs::double_variable_info::double_variable_info() :
    verification_type_info(3)
{}
codesh::output::jvm_target::defs::null_variable_info::null_variable_info() :
    verification_type_info(5)
{}
codesh::output::jvm_target::defs::uninitialized_this_variable_info::uninitialized_this_variable_info() :
    verification_type_info(6)
{}
codesh::output::jvm_target::defs::object_variable_info::object_variable_info() :
    verification_type_info(7)
{}
codesh::output::jvm_target::defs::uninitialized_variable_info::uninitialized_variable_info() :
    verification_type_info(8)
{}

codesh::output::jvm_target::defs::stack_map_frame::stack_map_frame(const unsigned char frame_type) :
    frame_type(frame_type)
{}

codesh::output::jvm_target::defs::stack_map_frame::~stack_map_frame() = default;

unsigned char codesh::output::jvm_target::defs::stack_map_frame::get_frame_type() const
{
    return frame_type;
}

codesh::output::jvm_target::defs::same_frame::same_frame(const unsigned char offset_delta) :
        stack_map_frame(offset_delta)
{
}

codesh::output::jvm_target::defs::same_locals_1_stack_item_frame::same_locals_1_stack_item_frame(
        const unsigned char offset_delta) : stack_map_frame(offset_delta + 64)
{
}

codesh::output::jvm_target::defs::same_locals_1_stack_item_frame_extended::same_locals_1_stack_item_frame_extended() :
    stack_map_frame(247)
{
}

codesh::output::jvm_target::defs::chop_frame::chop_frame(const unsigned char k) :
    stack_map_frame(251 - k)
{
}

codesh::output::jvm_target::defs::same_frame_extended::same_frame_extended() :
    stack_map_frame(251)
{
}

codesh::output::jvm_target::defs::append_frame::append_frame(const unsigned char k) :
    stack_map_frame(251 + k)
{
}

codesh::output::jvm_target::defs::full_frame::full_frame() :
    stack_map_frame(255)
{
}
