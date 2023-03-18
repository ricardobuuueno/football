#pragma once

#include "task_base.hpp"

namespace scanner
{

class new_championship_task : public task_base
{
  public:
    new_championship_task(pub::publisher pub, const std::string &url, const std::string &country,
                          const std::string &slots)
        : task_base()
    {
        type(task_type::new_championship);
        status(task_status::new_task);
        publisher(pub);

        set_property("url", url);
        set_property("country", country);
        set_property("slots", slots);
    }
};

using pnew_championship_task = std::unique_ptr<new_championship_task>;

} // namespace scanner