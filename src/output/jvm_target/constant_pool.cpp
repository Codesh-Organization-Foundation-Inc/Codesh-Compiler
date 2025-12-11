#include "constant_pool.h"

#include "../../defenition/definitions.h"
#include "../../util.h"
#include "../../blasphemies/blasphemy_collector.h"

#include <unordered_set>

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"

codesh::output::jvm_target::constant_pool::constant_pool(const ast::compilation_unit_ast_node &root_node,
        const ast::type_decl::type_declaration_ast_node &type_decl) :
    root_node(root_node),
    type_decl(type_decl),

    index(1),
    this_class_cpi(
        goc_class_info(
            goc_utf8_info(type_decl.get_binary_name())
        )
    )
{
    goc_utf8_info("SourceFile");
    goc_utf8_info(root_node.get_source_stem() + definition::SOURCE_FILE_EXTENSION);

    goc_utf8_info("Code");
    goc_utf8_info("LocalVariableTable");

    goc_utf8_info(type_decl.generate_descriptor());

    if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(&type_decl))
    {
        traverse_class_decl(*class_decl);
    }
}

void codesh::output::jvm_target::constant_pool::traverse_class_decl(
        const ast::type_decl::class_declaration_ast_node &class_decl)
{
    const int super_class_cpi = goc_utf8_info(
        class_decl.get_super_class()->get_binary_name()
    );

    const int super_class_constant = goc_class_info(super_class_cpi);

    // Add methods
    for (const auto &method_decl : class_decl.get_all_methods())
    {
        goc_methodref_info(
            this_class_cpi,

            goc_name_and_type_info(
                goc_utf8_info(method_decl->get_name()),
                goc_utf8_info(method_decl->generate_descriptor())
            )
        );

        // Add parameters
        for (const auto &param_node : method_decl->get_parameters())
        {
            goc_utf8_info(param_node->get_name());
            goc_utf8_info(param_node->get_type()->generate_descriptor());
        }
    }

    // Add super constructor method reference
    //TODO: Move to IR
    goc_methodref_info(
        super_class_constant,

        goc_name_and_type_info(
            goc_utf8_info("<init>"),
            //TODO: Actually check super call params
            goc_utf8_info("()V")
        )
    );
}


int codesh::output::jvm_target::constant_pool::goc_constant(std::unique_ptr<defs::cp_info> constant_info)
{
    const auto [it, inserted] = literals.emplace(std::move(constant_info));

    if (inserted)
    {
        literals_lookup_map.emplace(it->get(), index);
        return index++;
    }

    return literals_lookup_map.at(it->get());
}

int codesh::output::jvm_target::constant_pool::goc_utf8_info(const std::string &utf8)
{
    return goc_constant(utf8_info(utf8));
}

int codesh::output::jvm_target::constant_pool::goc_methodref_info(const int class_index, const int name_and_type_index)
{
    return goc_constant(methodref_info(class_index, name_and_type_index));
}
int codesh::output::jvm_target::constant_pool::goc_name_and_type_info(const int name_index, const int descriptor_index)
{
    return goc_constant(name_and_type_info(name_index, descriptor_index));
}
int codesh::output::jvm_target::constant_pool::goc_class_info(const int name_index)
{
    return goc_constant(class_info(name_index));
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Utf8_info>
    codesh::output::jvm_target::constant_pool::utf8_info(const std::string &utf8)
{
    if (utf8.size() > 0xFFFF)
    {
        error::blasphemy_collector().add_blasphemy("אֵין־זֶה ִכְתֻבִים—זוֹהִי מְגִלָּה! לְכָל הַיּוֹתֵר חָמֵשׁ תָּוִים וּשְׁלֹשִׁים תָּוִים וַחֲמֵשׁ מֵאוֹת וַחֲמִשָּׁה תָּוִים וְשִׁשִּׁים אָלֶף תָּוִים בִּכְתֻבִים",
            error::blasphemy_type::UNKNOWN, std::nullopt, true);
    }

    auto utf8_info = std::make_unique<defs::CONSTANT_Utf8_info>();
    util::put_int_bytes(utf8_info->length, 2, utf8.length()); // NOLINT(*-narrowing-conversions) (Handled overflow above)
    utf8_info->bytes.insert(utf8_info->bytes.end(), utf8.begin(), utf8.end());

    return std::move(utf8_info);
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Methodref_info>
    codesh::output::jvm_target::constant_pool::methodref_info(const int class_index, const int name_and_type_index)
{
    auto const_methodref = std::make_unique<defs::CONSTANT_Methodref_info>();

    util::put_int_bytes(const_methodref->class_index, 2, class_index);
    util::put_int_bytes(const_methodref->name_and_type_index, 2, name_and_type_index);

    return std::move(const_methodref);
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_NameAndType_info>
    codesh::output::jvm_target::constant_pool::name_and_type_info(const int name_index, const int descriptor_index)
{
    auto const_name_and_type = std::make_unique<defs::CONSTANT_NameAndType_info>();

    util::put_int_bytes(const_name_and_type->name_index, 2, name_index);
    util::put_int_bytes(const_name_and_type->descriptor_index, 2, descriptor_index);

    return std::move(const_name_and_type);
}

std::unique_ptr<codesh::output::jvm_target::defs::CONSTANT_Class_info>
    codesh::output::jvm_target::constant_pool::class_info(const int name_index)
{
    auto const_class = std::make_unique<defs::CONSTANT_Class_info>();

    util::put_int_bytes(const_class->name_index, 2, name_index);

    return std::move(const_class);
}


int codesh::output::jvm_target::constant_pool::get_index(const defs::cp_info &literal) const
{
    const auto result = literals_lookup_map.find(&literal);
    if (result == literals_lookup_map.end())
        throw std::runtime_error("Could not find literal in constant pool");

    return result->second;
}

int codesh::output::jvm_target::constant_pool::get_utf8_index(const std::string &utf8) const
{
    return get_index(*utf8_info(utf8));
}

int codesh::output::jvm_target::constant_pool::get_methodref_index(const int class_index,
                                                                   const int name_and_type_index) const
{
    return get_index(*methodref_info(class_index, name_and_type_index));
}

int codesh::output::jvm_target::constant_pool::get_name_and_type_index(const int name_index,
                                                                       const int descriptor_index) const
{
    return get_index(*name_and_type_info(name_index, descriptor_index));
}

int codesh::output::jvm_target::constant_pool::get_class_index(const int name_index) const
{
    return get_index(*class_info(name_index));
}


std::vector<std::reference_wrapper<const codesh::output::jvm_target::defs::cp_info>>
    codesh::output::jvm_target::constant_pool::get_literals() const
{
    // The literals need to match the order written in the Class file.
    // The order is specified by this map's int value.
    // So make the value the new key, and return the sorted vector.
    //
    // This then means that every return index (with get_index) matches the one in the class file.

    std::unordered_map<int, const defs::cp_info *> inverted_literals;
    for (const auto &[key, value] : literals_lookup_map)
    {
        inverted_literals.emplace(value, key);
    }


    std::vector<std::reference_wrapper<const defs::cp_info>> results;
    results.reserve(inverted_literals.size());

    for (int i = 0; i < inverted_literals.size(); i++)
    {
        results.emplace_back(*inverted_literals.at(i + 1));
    }

    return results;
}

size_t codesh::output::jvm_target::constant_pool::size() const
{
    return literals.size();
}
