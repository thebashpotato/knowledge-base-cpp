#include <cstddef>
#include <cstdint>
#include <string>

#include <data_structures/lru.hpp>
#include <data_structures/queue.hpp>
#include <data_structures/singley_linked_list.hpp>
#include <data_structures/stack.hpp>

#include <gtest/gtest.h>

using namespace Kbase::DataStructures;

TEST(SingleyLinkedList, TestPushFront)
{
    LinkedList<std::int32_t> list;

    list.PushFront(4);
    list.PushFront(3);
    list.PushFront(2);
    list.PushFront(1);

    const auto size_cache = list.Size();
    ASSERT_EQ(size_cache, 4);
}

TEST(SingleyLinkedList, TestPopFront)
{
    LinkedList<std::int32_t> list;

    list.PushFront(4);
    list.PushFront(3);
    list.PushFront(2);
    list.PushFront(1);

    const auto size_cache = list.Size();
    ASSERT_EQ(size_cache, 4);

    const auto opt = list.PopFront();
    ASSERT_EQ(opt.has_value(), true);
    ASSERT_EQ(opt.value(), 1);
    ASSERT_EQ(list.Size(), size_cache - 1);
}

TEST(SingleyLinkedList, TestPushBack)
{
    LinkedList<std::int32_t> list;

    list.PushBack(1);
    list.PushBack(2);
    list.PushBack(3);
    list.PushBack(4);
    list.PushBack(5);

    ASSERT_EQ(list.Size(), 5);
}

TEST(QueueTestSuite, TestEnqueue)
{
    Queue<std::int32_t> queue;

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    queue.Enqueue(4);
    queue.Enqueue(5);

    ASSERT_EQ(queue.Size(), 5);
}

TEST(QueueTestSuite, TestPeek)
{
    Queue<std::int32_t> queue;

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    queue.Enqueue(4);
    queue.Enqueue(5);

    ASSERT_EQ(queue.Size(), 5);
    const auto front = queue.Peek();
    ASSERT_EQ(front.has_value(), true);
    ASSERT_EQ(front.value(), 1);
    ASSERT_EQ(queue.Size(), 5);
}

TEST(QueueTestSuite, TestDeque)
{
    Queue<std::int32_t> queue;

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    queue.Enqueue(4);
    queue.Enqueue(5);

    ASSERT_EQ(queue.Size(), 5);
    const auto front = queue.Deque();
    ASSERT_EQ(front.has_value(), true);
    ASSERT_EQ(front.value(), 1);
    ASSERT_EQ(queue.Size(), 4);
}

TEST(StackTestSuite, TestPush)
{
    Stack<std::int32_t> stack;

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    stack.Push(4);
    stack.Push(5);

    ASSERT_EQ(stack.Size(), 5);
    ASSERT_EQ(stack.Empty(), false);
}

TEST(StackTestSuite, TestPop)
{
    Stack<std::int32_t> stack;

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    stack.Push(4);
    stack.Push(5);

    ASSERT_EQ(stack.Size(), 5);

    const auto size = stack.Size();

    for (std::size_t i = 0; i < size; ++i)
    {
        const auto opt = stack.Pop();
        ASSERT_EQ(opt.has_value(), true);
    }

    ASSERT_EQ(stack.Size(), 0);
    ASSERT_EQ(stack.Empty(), true);
}

TEST(LRUTestSuite, Test1)
{
    constexpr std::size_t max_size = 3;
    auto lru = LRUCache<std::string, int>(max_size);

    ASSERT_EQ(lru.MaxSize(), max_size);
    ASSERT_EQ(lru.Size(), 0);
    ASSERT_EQ(lru.Empty(), true);

    ASSERT_EQ(lru.Get("foo").has_value(), false);

    lru.Update("foo", 69);
    ASSERT_EQ(lru.Get("foo").has_value(), true);
    ASSERT_EQ(lru.Get("foo").value(), 69);

    lru.Update("bar", 420);
    ASSERT_EQ(lru.Get("bar").has_value(), true);
    ASSERT_EQ(lru.Get("bar").value(), 420);

    lru.Update("bazz", 820);
    ASSERT_EQ(lru.Get("bazz").has_value(), true);
    ASSERT_EQ(lru.Get("bazz").value(), 820);

    lru.Update("ball", 69420);
    ASSERT_EQ(lru.Get("ball").has_value(), true);
    ASSERT_EQ(lru.Get("ball").value(), 69420);

    ASSERT_EQ(lru.Get("foo").has_value(), false);
    ASSERT_EQ(lru.Get("bar").value(), 420);

    lru.Update("foo", 69);
    ASSERT_EQ(lru.Get("bar").has_value(), true);
    ASSERT_EQ(lru.Get("foo").value(), 69);

    // shouldn't of been deleted, but since bar was get'd, bar was added to the
    // front of the list, so bazz became the end
    ASSERT_EQ(lru.Get("bazz").has_value(), false);
}
