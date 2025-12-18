#include "resolve.h"

#include "../../parser/ast/method/operation/method_call_ast_node.h"
#include "../semantic_context.h"
#include "../util.h"
#include "fmt/color.h"

void codesh::semantic_analyzer::method_call::resolve(const semantic_context &context,
        const ast::method::method_declaration_ast_node &method_decl,
        method_symbol &method)
{
    for (auto &expression : method_decl.get_body())
    {
        auto *method_call = dynamic_cast<ast::method::operation::method_call_ast_node *>(expression.get());
        if (method_call == nullptr)
            continue;

        const auto resolved_name = util::resolve_method_call(context.lookup_countries, method_call->get_fqcn());
        if (!resolved_name.has_value())
        {
            context.blasphemy_consumer(fmt::format(
                "עֶצֶם בִּלְתִּי מְזֹהֶה: סוּג לֹא יָדוּעַ {}",
                method_call->get_name().join()
            ));
        }

        method_call->set_resolved_name(resolved_name.value());
    }
}
