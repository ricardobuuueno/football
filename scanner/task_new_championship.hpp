#pragma once

#include "task_base.hpp"
#include "task_new_championship.hpp"

#include "../pub/runner.hpp"

namespace scanner
{

class new_championship_task : public task_base
{
  public:
    explicit new_championship_task(const std::string &id) : task_base(id)
    {
    }

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

    auto run() -> std::tuple<bool, std::string, ptask_result> override
    {
        auto url = get_property("url");
        auto search_result = pub::get_new_championship(publisher(), url);

        auto success = std::get<0>(search_result);
        auto message = std::get<1>(search_result);

        if (success)
        {
            auto championship = std::get<2>(search_result);
            auto country = get_property("country");
            auto slots = get_property("slots");

            ptask_result result = football::New<result_new_championship>(championship, country, slots);

            return {success, message, std::move(result)};    
        }

        return {success, message, nullptr};
    }
};

using pnew_championship_task = std::unique_ptr<new_championship_task>;

} // namespace scanner