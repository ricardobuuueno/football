#pragma once

#include "../net/event.hpp"

#include <memory>

namespace scanner
{

class task_result
{
  public:
    virtual ~task_result() = default;

  private:
};

class result_new_championship : public task_result
{
};

using ptask_result = std::unique_ptr<task_result>;

} // namespace scanner