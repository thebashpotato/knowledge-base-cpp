#pragma once

#include "node.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

namespace Kbase::DataStructures
{

template <typename T> class Queue
{
  public:
    Queue() noexcept = default;

    /// @brief Appends a new value to the end of the list.
    auto Enqueue(const T &value) noexcept -> void
    {
        const auto new_node = std::make_shared<Node<T>>(value);
        if (this->Empty())
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

    /// @brief Removes a value from the front of the list.
    [[nodiscard]] auto Deque() noexcept -> std::optional<T>
    {
        std::optional<T> opt;
        if (!this->Empty())
        {
            const auto node = m_head;
            m_head = m_head->next;
            opt.emplace(std::move(node->value));
            --m_size;
        }

        return opt;
    }

    /// @brief Peek at the data held by the head node.
    [[nodiscard]] auto Peek() noexcept -> std::optional<T>
    {
        std::optional<T> opt;
        if (m_head != nullptr)
        {
            opt.emplace(m_head->value);
        }

        return opt;
    }

    [[nodiscard]] auto size() const noexcept -> std::size_t
    {
        return m_size;
    }

    [[nodiscard]] auto Empty() const noexcept -> bool
    {
        return m_head == nullptr && m_tail == nullptr;
    }

    auto Print() noexcept -> void
    {
        auto node = m_head;
        while (node != nullptr)
        {
            std::cout << node->value << ", ";
            node = node->next;
        }

        std::cout << '\n';
    }

  private:
    std::shared_ptr<Node<T>> m_head;
    std::shared_ptr<Node<T>> m_tail;
    std::size_t m_size{};
};

} // namespace Kbase::DataStructures
