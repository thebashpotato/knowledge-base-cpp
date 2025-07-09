#pragma once

#include <memory>
#include <utility>

namespace Kbase::DataStructures
{

template <typename T> struct Node
{
    explicit Node(T val) : value(std::move(val)), next(nullptr), prev(nullptr)
    {
    }

    T value;
    std::shared_ptr<Node<T>> next;
    std::shared_ptr<Node<T>> prev;
};

} // namespace Kbase::DataStructures
