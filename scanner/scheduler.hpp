#pragma once

#include "../db/mysql.hpp"
#include "../net/tsqueue.hpp"
#include "../util/globals.hpp"
#include "../util/util.hpp"

#include "task_base.hpp"
#include "task_new_championship.hpp"
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
    net::stsqueue<scanner::ptask_result> _queue;

    auto thread_run() -> void
    {
        auto c = _queue->count();
        auto u = _queue.use_count();
        std::cout << "count: " << c << '\n';
        std::cout << "use count: " << u << '\n';

        const std::string tasks_stmt = fmt::format("SELECT id, type FROM tasks WHERE status = {}",
                                                   static_cast<uint64_t>(scanner::task_status::new_task));

        while (true)
        {
            auto json = mysql::srv->prepare_execute(tasks_stmt);
            std::cout << json << '\n';

            std::vector<ptask_base> tasks{};

            auto tbl = nlohmann::json::parse(json);
            for (auto &row : tbl)
            {
                std::cout << row["id"] << '\n';
                std::string sid = row["id"].dump();
                sid.erase(remove(sid.begin(), sid.end(), '\"'), sid.end());
                std::string stype = row["type"].dump();
                stype.erase(remove(stype.begin(), stype.end(), '\"'), stype.end());
                std::cout << stype << '\n';
                auto type = std::stoul(stype, 0, 10);

                switch (static_cast<scanner::task_type>(type))
                {
                case scanner::task_type::new_championship: {
                    auto task = util::New<scanner::new_championship_task>(sid);
                    tasks.emplace_back(std::move(task));
                }
                break;
                }
            }

            for (auto &task : tasks)
            {
                task->status(scanner::task_status::running);
                if (task->save())
                {
                    auto task_run_result = task->run();

                    if (auto success = std::get<0>(task_run_result); success)
                    {
                        auto result = std::move(std::get<2>(task_run_result));

                        task->results(result->json());
                        task->status(scanner::task_status::finished);
                        if (task->save())
                        {
                            _queue->push_back(std::move(result));
                        }
                    }
                    else
                    {
                        auto message = std::move(std::get<1>(task_run_result));
                        task->results(message);
                        task->status(scanner::task_status::error);
                        auto saved = task->save();
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));

            if (cancel_thread)
            {
                break;
            }
        }
    }
};

using pscheduler = std::unique_ptr<scheduler>;

} // namespace scanner