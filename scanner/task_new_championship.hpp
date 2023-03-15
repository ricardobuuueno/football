#pragma once

#include "task_base.hpp"

namespace scanner
{

class new_championship_task : public task_base
{
  public:
    new_championship_task(const std::string &url, const std::string &country, uint8_t slots)
    {
    }
};

using pnew_championship_task = std::unique_ptr<new_championship_task>;

} // namespace scanner