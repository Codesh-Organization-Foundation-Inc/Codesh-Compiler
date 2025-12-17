#include "resolve.h"

#include "../../parser/ast/method/operation/method_call_ast_node.h"
#include "../util.h"

void codesh::semantic_analyzer::method_call::resolve(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        const ast::method::method_declaration_ast_node &method_decl,
        method_symbol &method)
{
    for (auto &expression : method_decl.get_body())
    {
        auto *method_call = dynamic_cast<ast::method::operation::method_call_ast_node *>(expression.get());
        if (method_call == nullptr)
            continue;

        const auto resolved_name = util::resolve_method_call(lookup_countries, method_call->get_fqcn());
        if (!resolved_name.has_value())
        {
            //FIXME: DONT THROW, WRAP ERROR AS BLASPHEMY
            throw std::runtime_error("עֶצֶם בִּלְתִּי מְזֹהֶה: סוּג לֹא יָדוּעַ " + method_call->get_name());
        }

        method_call->set_resolved_name(resolved_name->join());
    }
}
