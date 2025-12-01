#include "i_descriptor_emitter.h"

codesh::ast::impl::i_descriptor_emitter::~i_descriptor_emitter() = default;

std::string codesh::ast::impl::i_descriptor_emitter::generate_descriptor() const
{
    return generate_descriptor(true);
}
