#pragma once

template <typename T>
codesh::semantic_analyzer::i_ast_produced<T>::~i_ast_produced() = default;

template <typename T>
bool codesh::semantic_analyzer::i_ast_produced<T>::is_external() const
{
    return get_producing_node() == nullptr;
}
