#pragma once
#include <algorithm>
#include <memory>

namespace codesh::output::jvm_target
{

struct attribute_info
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];

    virtual ~attribute_info();
};

struct exception_table
{
    unsigned char start_pc[2];
    unsigned char end_pc[2];
    unsigned char handler_pc[2];
    unsigned char catch_type[2];
};

struct Code_attribute : attribute_info
{
    unsigned char max_stack[2];
    unsigned char max_locals[2];
    unsigned char code_length[4];
    // unsigned char exception_table_length[2];
    std::vector<std::unique_ptr<exception_table>> exception_table;
    // unsigned char attribute_count[2];
    std::vector<std::unique_ptr<attribute_info>> attributes;
};

struct line_number_table
{
    unsigned char start_pc[2];
    unsigned char line_number[2];
};

struct LineNumberTable_attribute : attribute_info
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];
    // unsigned char line_number_table_length[2];
    std::vector<std::unique_ptr<line_number_table>> line_number_table;
};

struct local_variable_table
{
    unsigned char start_pc[2];
    unsigned char length[2];
    unsigned char name_index[2];
    unsigned char descriptor_index[2];
    unsigned char index[2];
};

struct LocalVariableTable_attribute : attribute_info
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];
    // unsigned char local_variable_table_length[2];
    std::vector<std::unique_ptr<line_number_table>> line_number_table;
};

struct SourceFile_attribute : attribute_info
{
    unsigned char attribute_name_index[2];
    unsigned char attribute_length[4];
    unsigned char sourcefile_index[2];
};
}