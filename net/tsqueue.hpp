#pragma once

#include "common.hpp"

namespace net
{

template <typename T> class tsqueue
{
  public:
    tsqueue() = default;
    tsqueue(const tsqueue<T> &) = delete;

    virtual ~tsqueue()
    {
        clear();
    }

    const T &front()
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.front();
    }

    const T &back()
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.back();
    }

    T pop_front()
    {
        std::scoped_lock lock(queue_mutex);
        auto t = std::move(ts_queue.front());
        ts_queue.pop_front();
        return t;
    }

    T pop_back()
    {
        std::scoped_lock lock(queue_mutex);
        auto t = std::move(ts_queue.back());
        ts_queue.pop_back();
        return t;
    }

    void push_back(const T &item)
    {
        std::scoped_lock lock(queue_mutex);
        ts_queue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(blocking_mutex);
        cv_wait.notify_one();
    }

    void push_front(const T &item)
    {
        std::scoped_lock lock(queue_mutex);
        ts_queue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(blocking_mutex);
        cv_wait.notify_one();
    }

    bool empty()
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.empty();
    }

    size_t count()
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.size();
    }

    void clear()
    {
        std::scoped_lock lock(queue_mutex);
        ts_queue.clear();
    }

    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> ul(blocking_mutex);
            cv_wait.wait(ul);
        }
    }

  protected:
    std::mutex queue_mutex;
    std::deque<T> ts_queue;
    std::condition_variable cv_wait;
    std::mutex blocking_mutex;
};

} // namespace net