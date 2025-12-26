#pragma once

namespace codesh::output::jvm_target
{
class constant_pool;
}


namespace codesh::ast::impl
{

class i_constant_pool_emitter
{
public:
    virtual ~i_constant_pool_emitter() = default;

    virtual void emit_constant(output::jvm_target::constant_pool &constant_pool) const = 0;
};

}
