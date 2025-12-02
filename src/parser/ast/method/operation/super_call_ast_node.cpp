#include "super_call_ast_node.h"

#include "../../../../output/ir/code_block.h"
#include "../../type_declaration/class_declaration_ast_node.h"

void codesh::ast::method::operation::super_call_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::class_declaration_ast_node &containing_class_decl) const
{
    const output::jvm_target::constant_pool &constant_pool = containing_class_decl.get_constant_pool().value();

    const int method_cp_index = constant_pool.get_methodref_index(
        //TODO: Use actual superclass
        constant_pool.get_class_index(constant_pool.get_utf8_index("java/lang/Object")),

        constant_pool.get_name_and_type_index(
            constant_pool.get_utf8_index("<init>"),
            constant_pool.get_utf8_index("()V")
        )
    );

    containing_block.add_instruction(std::make_unique<output::ir::invoke_special_instruction>(method_cp_index));
}
