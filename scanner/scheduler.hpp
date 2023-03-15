#pragma once

#include "../net/tsqueue.hpp"
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
    }

    auto run() -> void
    {
    }

    auto wait() -> void
    {
    }

  private:
    net::stsqueue<ptask_result> _queue;
};

using pscheduler = std::unique_ptr<scheduler>;

} // namespace scanner