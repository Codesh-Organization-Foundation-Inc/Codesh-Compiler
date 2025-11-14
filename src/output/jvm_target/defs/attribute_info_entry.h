#pragma once
#include <memory>
#include <vector>

namespace codesh::output::jvm_target::defs
{

struct attribute_info_entry
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];

    virtual ~attribute_info_entry();
};

struct exception_table_entry
{
    unsigned char start_pc[2];
    unsigned char end_pc[2];
    unsigned char handler_pc[2];
    unsigned char catch_type[2];
};

struct code_attribute_entry : attribute_info_entry
{
    unsigned char max_stack[2];
    unsigned char max_locals[2];
    unsigned char code_length[4];
    unsigned char code[5]; // for now
    unsigned char exception_table_length[2];
    std::vector<std::unique_ptr<exception_table_entry>> exception_table;
    unsigned char attribute_count[2];
    std::vector<std::unique_ptr<attribute_info_entry>> attributes;
};

struct line_number_table_entry
{
    unsigned char start_pc[2];
    unsigned char line_number[2];
};

struct line_number_table_attribute_entry : attribute_info_entry
{
    unsigned char line_number_table_length[2];
    std::vector<std::unique_ptr<line_number_table_entry>> line_number_table;
};

struct local_variable_table_entry
{
    unsigned char start_pc[2];
    unsigned char length[2];
    unsigned char name_index[2];
    unsigned char descriptor_index[2];
    unsigned char index[2];
};

struct local_variable_table_attribute_entry : attribute_info_entry
{
    unsigned char local_variable_table_length[2];
    std::vector<std::unique_ptr<local_variable_table_entry>> local_variable_table;
};

struct source_file_attribute_entry : attribute_info_entry
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];
    unsigned char sourcefile_index[2];
};
}