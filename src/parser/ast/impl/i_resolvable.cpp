#include "i_resolvable.h"

#include <iostream>

codesh::ast::impl::i_resolvable::~i_resolvable() = default;

void codesh::ast::impl::i_resolvable::set_resolved_name(std::string resolved_name)
{
    _get_resolved_name().emplace(std::move(resolved_name));
}

std::string codesh::ast::impl::i_resolvable::get_resolved_name() const
{
    const auto &resolved_name = _get_resolved_name();

    if (!resolved_name)
        throw std::runtime_error("Attempted to get resolved name while none exists");

    return resolved_name.value();
}

std::string codesh::ast::impl::i_resolvable::get_binary_name(const bool resolved) const
{
    if (!resolved)
        return get_name();

    //TODO: This should THROW if the name is not resolved.
    if (!_get_resolved_name())
    {
        std::cerr << "NOTICE: USING UNRESOLVED NAME " << get_name() << std::endl;
        return get_name();
    }

    return _get_resolved_name().value();
}
