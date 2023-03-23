#pragma once

#include "../net/event.hpp"

#include <memory>

namespace scanner
{

class task_result
{
  public:
    explicit task_result(){};
    task_result(const task_result &) = delete;
    task_result(task_result &&) = default;
    task_result &operator=(const task_result &) = delete;
    task_result &operator=(task_result &&) = default;
    virtual ~task_result() = default;

  private:
};

class result_new_championship : public task_result
{
};

using ptask_result = std::unique_ptr<task_result>;

} // namespace scanner