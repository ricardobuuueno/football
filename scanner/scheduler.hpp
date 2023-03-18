#pragma once

#include "../db/mysql.hpp"
#include "../net/tsqueue.hpp"
#include "task_base.hpp"
#include "task_result.hpp"

#include <memory>
#include <string>
#include <vector>

namespace scanner
{

class scheduler
{
  public:
    explicit scheduler(net::stsqueue<ptask_result> queue) : _queue(queue)
    {
        sched_future = std::async(std::launch::async, &scheduler::thread_run, this);
    }

    ~scheduler()
    {
        cancel_thread = true;
        sched_future.get();
    }

  private:
    std::future<void> sched_future;
    std::atomic_bool cancel_thread{ATOMIC_VAR_INIT(false)};
    net::stsqueue<ptask_result> _queue;

    auto thread_run() -> void
    {
        auto c = _queue->count();
        auto u = _queue.use_count();
        std::cout << "count: " << c << '\n';
        std::cout << "use count: " << u << '\n';

        const std::string tasks_stmt = fmt::format("SELECT Id FROM tasks WHERE status = {}",
                                                   static_cast<uint64_t>(scanner::task_status::new_task));

        while (true)
        {
            auto json = mysql::srv->prepare_execute(tasks_stmt);
            std::cout << json << '\n';

            std::this_thread::sleep_for(std::chrono::seconds(10));

            if (cancel_thread)
            {
                break;
            }
        }
    }
};

using pscheduler = std::unique_ptr<scheduler>;

} // namespace scanner