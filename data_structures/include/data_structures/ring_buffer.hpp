#pragma once

#include <cstddef>
#include <optional>
#include <vector>

namespace Kbase::DataStructures
{

/// @brief A circular buffer, it has a fixed Size which wraps around upon reaching
/// the end, effectively using the same memory space in a cyclic manner. This structure
/// is particulary usefull in situtations where data is continuously produced and consumed
/// at different rates.
template <typename T> class RingBuffer
{
  public:
    explicit RingBuffer(const std::size_t Size) : m_buffer(Size), m_capacity(Size)
    {
    }

    /// @brief Insert an element at the tail pointer. If the buffer is full, the tail wraps
    /// around to the beginning of the buffer.
    auto Enqueue(T item) noexcept -> void
    {
        m_buffer[m_tail] = item;
        if (m_is_full)
        {
            m_head = (m_head + 1) % m_capacity;
        }
        m_tail = (m_tail + 1) % m_capacity;
        m_is_full = m_tail == m_head;
    }

    /// @brief Removes an element from the head pointer.
    ///
    /// @details If the buffer is empty, the head pointer also wraps around.
    auto Dequeue() noexcept -> std::optional<T>
    {
        std::optional<T> ret;
        if (!this->Empty())
        {
            const T item = m_buffer[m_head];
            m_head = (m_head + 1) % m_capacity;
            m_is_full = false;
            ret.emplace(item);
        }
        return ret;
    }

    [[nodiscard]] auto Empty() const noexcept -> bool
    {
        return (!m_is_full && (m_head == m_tail));
    }

    [[nodiscard]] auto Size() const noexcept -> std::size_t
    {
        std::size_t Size = m_capacity;

        if (!m_is_full)
        {
            if (m_tail >= m_head)
            {
                Size = m_tail - m_head;
            }
            else
            {
                Size = m_capacity + m_tail - m_head;
            }
        }
        return Size;
    }

  private:
    // underlying data structure we are using
    std::vector<T> m_buffer;
    // fixed Size
    std::size_t m_capacity;
    // read pointer
    std::size_t m_head{};
    // write pointer
    std::size_t m_tail{};
    bool m_is_full{};
};

} // namespace Kbase::DataStructures
