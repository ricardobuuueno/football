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

    auto front() -> const T &
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.front();
    }

    auto back() -> const T &
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.back();
    }

    auto pop_front() -> T
    {
        std::scoped_lock lock(queue_mutex);
        auto t = std::move(ts_queue.front());
        ts_queue.pop_front();
        return t;
    }

    auto pop_back() -> T
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

    void push_back(T &&item)
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

    void push_front(T &&item)
    {
        std::scoped_lock lock(queue_mutex);
        ts_queue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(blocking_mutex);
        cv_wait.notify_one();
    }

    auto empty() -> bool
    {
        std::scoped_lock lock(queue_mutex);
        return ts_queue.empty();
    }

    auto count() -> size_t
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

template <typename T> using ptsqueue = std::unique_ptr<tsqueue<T>>;
template <typename T> using stsqueue = std::shared_ptr<tsqueue<T>>;

} // namespace net