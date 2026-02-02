#include "class_file_writer.h"

#include <filesystem>
#include <fmt/xchar.h>
#include <fstream>

#include <ranges>

#include <vector>

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "output/jvm_target/defs/methods_info_entry.h"

#include "output/jvm_target/defs/attribute_info_entry.h"
#include "output/jvm_target/defs/class_file.h"

static void write_bytes(std::ofstream &out, const unsigned char *data, std::streamsize length);
static void write_methods(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods);
static void write_attributes(std::ofstream &out,
        const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes);
static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file);
static void write_verification_type(std::ofstream &out,
        const codesh::output::jvm_target::defs::verification_type_info &info);
static void write_stack_map_frame(std::ofstream &out,
        const codesh::output::jvm_target::defs::stack_map_frame &frame);


void codesh::output::jvm_target::write_to_file(const defs::class_file &class_file,
                                               const ast::type_decl::type_declaration_ast_node &type_decl,
                                               const std::filesystem::path &destination)
{
    std::ofstream destination_file(destination / (type_decl.get_last_name(false) + ".class"), std::ios::binary);

    if (!destination_file)
    {
        blasphemy::blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::SOURCE_FILE_OPEN_ERROR,
                (destination / (type_decl.get_last_name(false) + ".class")).string()
            ),
            blasphemy::blasphemy_type::OUTPUT,
            std::nullopt,
            true
        );
    }


    write_bytes(destination_file, class_file.magic, 4);
    write_bytes(destination_file, class_file.minor_version, 2);
    write_bytes(destination_file, class_file.major_version, 2);
    write_bytes(destination_file, class_file.constant_pool_count, 2);

    write_constant_pool(destination_file, class_file);

    write_bytes(destination_file, class_file.access_flags, 2);
    write_bytes(destination_file, class_file.this_class, 2);
    write_bytes(destination_file, class_file.super_class, 2);

    write_bytes(destination_file, class_file.interfaces_count, 2);
    write_bytes(destination_file, class_file.fields_count, 2);
    write_bytes(destination_file, class_file.methods_count, 2);

    write_methods(destination_file, class_file.methods_info);

    write_bytes(destination_file, class_file.attribute_count, 2);
    write_attributes(destination_file, class_file.attribute_info);

    destination_file.close();
}

static void write_bytes(std::ofstream &out, const unsigned char *data, const std::streamsize length)
{
    out.write(reinterpret_cast<const char *>(data), length);
}

static void write_constant_pool(std::ofstream &out, const codesh::output::jvm_target::defs::class_file &class_file)
{
    for (const auto &info : class_file.constant_pool)
    {
        write_bytes(out, &info.get().tag, 1);

        if (const auto utf8_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Utf8_info *>(&info.get()))
        {
            write_bytes(out, utf8_info->length, 2);
            write_bytes(out, utf8_info->bytes.data(), utf8_info->bytes.size()); // NOLINT(*-narrowing-conversions) (Already checked in add_utf8_info)
        }
        else if (const auto cls_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Class_info *>(&info.get()))
        {
            write_bytes(out, cls_info->name_index, 2);
        }
        else if (const auto mref_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Methodref_info *>(&info.get()))
        {
            write_bytes(out, mref_info->class_index, 2);
            write_bytes(out, mref_info->name_and_type_index, 2);
        }
        else if (const auto nat_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_NameAndType_info *>(&info.get()))
        {
            write_bytes(out, nat_info->name_index, 2);
            write_bytes(out, nat_info->descriptor_index, 2);
        }
        else if (const auto integer_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Integer_info *>(&info.get()))
        {
            write_bytes(out, integer_info->bytes, 4);
        }
        else if (const auto string_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_String_info *>(&info.get()))
        {
            write_bytes(out, string_info->string_index, 2);
        }
        else if (const auto fieldref_info = dynamic_cast<const codesh::output::jvm_target::defs::CONSTANT_Fieldref_info *>(&info.get()))
        {
            write_bytes(out, fieldref_info->class_index, 2);
            write_bytes(out, fieldref_info->name_and_type_index, 2);
        }
        else
        {
            throw std::runtime_error("Unknown constant pool type");
        }
    }
}

static void write_verification_type(std::ofstream &out, const codesh::output::jvm_target::defs::verification_type_info &info)
{
    const unsigned char tag = info.get_tag();
    write_bytes(out, &tag, 1);

    if (const auto object_info = dynamic_cast<const codesh::output::jvm_target::defs::object_variable_info *>(&info))
    {
        write_bytes(out, object_info->cpool_index, 2);
    }
    else if (const auto uninitialized_info = dynamic_cast<const codesh::output::jvm_target::defs::uninitialized_variable_info *>(&info))
    {
        write_bytes(out, uninitialized_info->offset, 2);
    }
}

static void write_stack_map_frame(std::ofstream &out, const codesh::output::jvm_target::defs::stack_map_frame &frame)
{
    const unsigned char frame_type = frame.get_frame_type();
    write_bytes(out, &frame_type, 1);

    if (dynamic_cast<const codesh::output::jvm_target::defs::same_frame *>(&frame))
    {
        return;
    }
    if (const auto same_stack = dynamic_cast<const codesh::output::jvm_target::defs::same_locals_1_stack_item_frame *>(&frame))
    {
        write_verification_type(out, *same_stack->stack);
        return;
    }
    if (const auto same_stack_ext = dynamic_cast<const codesh::output::jvm_target::defs::same_locals_1_stack_item_frame_extended *>(&frame))
    {
        write_bytes(out, same_stack_ext->offset_delta, 2);
        write_verification_type(out, *same_stack_ext->stack);
        return;
    }
    if (const auto chop = dynamic_cast<const codesh::output::jvm_target::defs::chop_frame *>(&frame))
    {
        write_bytes(out, chop->offset_delta, 2);
        return;
    }
    if (const auto same_ext = dynamic_cast<const codesh::output::jvm_target::defs::same_frame_extended *>(&frame))
    {
        write_bytes(out, same_ext->offset_delta, 2);
        return;
    }
    if (const auto append = dynamic_cast<const codesh::output::jvm_target::defs::append_frame *>(&frame))
    {
        write_bytes(out, append->offset_delta, 2);
        for (const auto &local : append->locals)
        {
            write_verification_type(out, *local);
        }

        return;
    }
    if (const auto full = dynamic_cast<const codesh::output::jvm_target::defs::full_frame *>(&frame))
    {
        write_bytes(out, full->offset_delta, 2);
        write_bytes(out, full->number_of_locals, 2);
        for (const auto &local : full->locals)
        {
            write_verification_type(out, *local);
        }
        write_bytes(out, full->number_of_stack_items, 2);
        for (const auto &stack_item : full->stack)
        {
            write_verification_type(out, *stack_item);
        }

        return;
    }

    throw std::runtime_error("Unknown stack map frame type");
}

static void write_attributes(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::attribute_info_entry>> &attributes)
{
    for (const auto &attr : attributes)
    {
        write_bytes(out, attr->attribute_name_index, 2);
        write_bytes(out, attr->attribute_length, 4);

        if (const auto code_attr = dynamic_cast<const codesh::output::jvm_target::defs::code_attribute_entry *>(attr.get()))
        {
            write_bytes(out, code_attr->max_stack, 2);
            write_bytes(out, code_attr->max_locals, 2);
            write_bytes(out, code_attr->code_length, 4);
            write_bytes(out, code_attr->code.data(), code_attr->code.size()); // NOLINT(*-narrowing-conversions)
            write_bytes(out, code_attr->exception_table_length, 2);

            write_bytes(out, code_attr->attribute_count, 2);
            write_attributes(out, code_attr->attributes);
        }
        else if (const auto lnt_attr = dynamic_cast<const codesh::output::jvm_target::defs::line_number_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lnt_attr->line_number_table_length, 2);
            for (const auto &entry : lnt_attr->line_number_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->line_number, 2);
            }
        }
        else if (const auto lvt_attr = dynamic_cast<const codesh::output::jvm_target::defs::local_variable_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, lvt_attr->local_variable_table_length, 2);
            for (const auto &entry : lvt_attr->local_variable_table)
            {
                write_bytes(out, entry->start_pc, 2);
                write_bytes(out, entry->length, 2);
                write_bytes(out, entry->name_index, 2);
                write_bytes(out, entry->descriptor_index, 2);
                write_bytes(out, entry->index, 2);
            }
        }
        else if (const auto smt_attr = dynamic_cast<const codesh::output::jvm_target::defs::stack_map_table_attribute_entry *>(attr.get()))
        {
            write_bytes(out, smt_attr->number_of_entries, 2);
            for (const auto &frame : smt_attr->entries)
            {
                write_stack_map_frame(out, *frame);
            }
        }
        else if (const auto src_attr = dynamic_cast<const codesh::output::jvm_target::defs::source_file_attribute_entry *>(attr.get()))
        {
            write_bytes(out, src_attr->sourcefile_index, 2);
        }
        else
        {
            throw std::runtime_error("Unknown attribute type");
        }
    }
}

static void write_methods(std::ofstream &out, const std::vector<std::unique_ptr<codesh::output::jvm_target::defs::methods_info_entry>> &methods)
{
    for (const auto &method : methods)
    {
        write_bytes(out, method->access_flags, 2);
        write_bytes(out, method->name_index, 2);
        write_bytes(out, method->descriptor_index, 2);
        write_bytes(out, method->attributes_count, 2);
        write_attributes(out, method->attribute_info);
    }
}
