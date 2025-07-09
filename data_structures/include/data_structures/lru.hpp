#pragma once

#include "node.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <unordered_map>

namespace Kbase::DataStructures
{

/// LRU (Least Recently Used) Cache this means we will evict the least recently used item.
///
/// To create a LRU you need two data structres, a Doubly Linked List and a Hash Map.
template <typename K, typename V> class LRUCache
{
  public:
    explicit LRUCache(const std::size_t max_size) noexcept : m_max_size(max_size)
    {
        m_lookup.reserve(max_size);
        m_reverse_lookup.reserve(max_size);
    }

  public:
    auto Update(const K &key, const V &value) noexcept -> void
    {
        std::shared_ptr<Node<V>> node;
        if (m_lookup.contains(key))
        {
            // if it does exist:
            //  - Update the value
            //  - Move the node to the front of the list
            node = m_lookup.at(key);
            node->value = value;
            this->Detach(node);
            this->Prepend(node);
        }
        else
        {
            // If the the key does not exist, we need to create a new node.
            //  - A newly created node becomes the most recently used element in the cache, thus it must automatically
            //    be prepended to the front of the list.
            //
            //  - Before the node can be added, the max_size must be checked, if we are at capacity the last element
            //    (Least recently used) must be evicted
            node = std::make_shared<Node<V>>(std::move(value));
            m_size++;
            this->Prepend(node);
            this->TrimCache();
            m_lookup.insert({key, node});
            m_reverse_lookup.insert({node, key});
        }
    }

    auto Get(const K &key) noexcept -> std::optional<V>
    {
        std::optional<V> opt;

        // check the cache for existence
        if (m_lookup.contains(key))
        {
            auto node = m_lookup.at(key);

            // update the value we found and move it to the front (if we are getting it , it is now the most recently
            // used)
            this->Detach(node);
            this->Prepend(node);
            opt.emplace(node->value);
        }

        // return out the value found, or undefined if the key is not found
        return opt;
    }

  public:
    /// @brief Returns the number of elements in the cache
    [[nodiscard]] auto size() const noexcept -> std::size_t
    {
        return m_size;
    }

    /// @brief Returns the maximum number of elements in the cache
    [[nodiscard]] auto max_size() const noexcept -> std::size_t
    {
        return m_max_size;
    }

    [[nodiscard]] auto Empty() const noexcept -> bool
    {
        return m_size == 0;
    }

  private:
    auto Detach(std::shared_ptr<Node<V>> &node) noexcept -> void
    {
        if (node->prev)
        {
            node->prev->next = node->next;
        }

        if (node->next)
        {
            node->next->prev = node->prev;
        }

        // check head and tail

        // The node that is being detached is the head
        if (m_head == node)
        {
            m_head = m_head->next;
        }

        // The node that is being detached is the tail
        if (m_tail == node)
        {
            m_tail = m_tail->prev;
        }

        node->next = nullptr;
        node->prev = nullptr;
    }

    auto Prepend(std::shared_ptr<Node<V>> &node) noexcept -> void
    {
        // The list is empty,
        if (m_head == nullptr)
        {
            m_head = m_tail = node;
        }
        else
        {
            node->next = m_head;
            m_head->prev = node;
            m_head = node;
        }
    }

    auto TrimCache() noexcept -> void
    {
        if (m_size <= m_max_size)
        {
            return;
        }

        auto current_tail = m_tail;
        this->Detach(current_tail);

        if (m_reverse_lookup.contains(current_tail))
        {
            const K key = m_reverse_lookup.at(current_tail);
            m_lookup.erase(key);
            m_reverse_lookup.erase(current_tail);
            --m_size;
        }
    }

  private:
    std::shared_ptr<Node<V>> m_head;
    std::shared_ptr<Node<V>> m_tail;
    std::unordered_map<K, std::shared_ptr<Node<V>>> m_lookup;
    std::unordered_map<std::shared_ptr<Node<V>>, K> m_reverse_lookup;
    std::size_t m_size{};
    std::size_t m_max_size{};
};

} // namespace Kbase::DataStructures
