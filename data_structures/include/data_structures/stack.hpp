#pragma once

#include "node.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

namespace Kbase::DataStructures
{

template <typename T> class Stack
{
  public:
    Stack() = default;

    /// @brief Appends data to the top of the stack
    auto Push(const T &value) noexcept -> void
    {
        const auto new_node = std::make_shared<Node<T>>(value);
        if (this->Empty())
        {
            m_head = new_node;
        }
        else
        {
            new_node->next = m_head;
            m_head = new_node;
        }

        ++m_size;
    }

    /// @brief Removes data from the top of the stack
    [[nodiscard]] auto Pop() noexcept -> std::optional<T>
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

    /// @brief Gets a copy of the data at the top of the stack
    [[nodiscard]] auto Peek() noexcept -> std::optional<T>
    {
        std::optional<T> opt;
        if (!this->Empty())
        {
            opt.emplace(m_head->value);
        }

        return opt;
    }

    /// @brief Gets the length of the stack
    [[nodiscard]] auto Size() noexcept -> std::size_t
    {
        return m_size;
    }

    /// @brief Is the stack empty
    [[nodiscard]] auto Empty() noexcept -> bool
    {
        return m_head == nullptr;
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
    std::size_t m_size{};
};

} // namespace Kbase::DataStructures
