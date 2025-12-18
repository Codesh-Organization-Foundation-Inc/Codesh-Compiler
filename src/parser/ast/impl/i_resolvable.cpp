#include "i_resolvable.h"

#include <iostream>

codesh::ast::impl::i_resolvable::~i_resolvable() = default;

const codesh::definition::fully_qualified_class_name &codesh::ast::impl::i_resolvable::get_name(const bool resolved)
    const
{
    if (resolved)
        return get_resolved_name();

    return get_name();
}

void codesh::ast::impl::i_resolvable::set_resolved_name(definition::fully_qualified_class_name resolved_name)
{
    _get_resolved_name().emplace(std::move(resolved_name));
}

const codesh::definition::fully_qualified_class_name &codesh::ast::impl::i_resolvable::get_resolved_name() const
{
    const auto &resolved_name = _get_resolved_name();

    if (!resolved_name)
    {
        //TODO: Throw:
        // throw std::runtime_error("Attempted to get resolved name while none exists");

        std::cerr << "NOTICE: USING UNRESOLVED NAME " << get_name().join() << std::endl;
        return get_name();
    }

    return resolved_name.value();
}
