#pragma once

#include <memory>

namespace scanner
{

class task_base
{
  public:
    [[nodiscard]] auto save() -> bool
    {
        return false;
    }
};

} // namespace scanner