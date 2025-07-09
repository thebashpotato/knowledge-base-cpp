#pragma once

#include "node.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

namespace Kbase::DataStructures
{

template <typename T> class LinkedList
{
  public:
    LinkedList() = default;

  public:
    [[nodiscard]] auto Empty() noexcept -> bool
    {
        return m_head == nullptr && m_tail == nullptr;
    }

    auto PushFront(const T &value) noexcept -> void
    {
        const auto new_node = std::make_shared<Node<T>>(value);
        if (Empty())
        {
            m_head = m_tail = new_node;
        }
        else
        {
            new_node->next = m_head;
            m_head = new_node;
        }
        ++m_size;
    }

    [[nodiscard]] auto PopFront() noexcept -> std::optional<T>
    {
        std::optional<T> opt;

        if (m_head != nullptr)
        {
            const auto temp = m_head;
            m_head = m_head->next;
            opt.emplace(std::move(temp->value));
            --m_size;
        }

        return opt;
    }

    auto PushBack(const T &value) noexcept -> void
    {
        const auto new_node = std::make_shared<Node<T>>(value);
        if (Empty())
        {
            m_head = m_tail = new_node;
        }
        else
        {
            m_tail->next = new_node;
            m_tail = new_node;
        }

        ++m_size;
    }

    [[nodiscard]] auto Size() noexcept -> std::size_t
    {
        return m_size;
    }

    auto Print() noexcept -> void
    {
        auto current = m_head;
        while (current != nullptr)
        {
            std::cout << current.value << ", ";
            current = current->next;
        }
        std::cout << '\n';
    }

  protected:
    std::shared_ptr<Node<T>> m_head;
    std::shared_ptr<Node<T>> m_tail;
    std::size_t m_size{};
};

} // namespace Kbase::DataStructures
