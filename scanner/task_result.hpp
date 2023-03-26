#pragma once

#include "../net/event.hpp"
#include "../util/json.hpp"

#include <memory>

namespace scanner
{

class task_result
{
  public:
    explicit task_result(net::event_type et) : _event_type(et){};
    task_result(const task_result &) = delete;
    task_result(task_result &&) = default;
    task_result &operator=(const task_result &) = delete;
    task_result &operator=(task_result &&) = default;
    virtual ~task_result() = default;

    virtual auto json() -> std::string = 0;

    [[nodiscard]] auto event_type() const -> net::event_type
    {
        return _event_type;
    }

  private:
    net::event_type _event_type;
};

class result_new_championship final : public task_result
{
  public:
    result_new_championship(const std::string &name, const std::string &country, const std::string &slots)
        : task_result(net::event_type::new_championship)
    {
        std::copy(name.begin(), name.end(), _championship.name.data());
        std::copy(country.begin(), country.end(), _championship.country.data());
    }

    [[nodiscard]] auto get() -> net::new_championship
    {
        return std::move(_championship);
    }

    [[nodiscard]] auto json() -> std::string override
    {
        nlohmann::json j;
        j["name"] = std::string(_championship.name.begin(),
                                std::find(_championship.name.begin(), _championship.name.end(), '\0'));
        j["country"] = std::string(std::begin(_championship.country), std::end(_championship.country));
        return j.dump();
    }

  private:
    net::new_championship _championship{};
};

using ptask_result = std::unique_ptr<task_result>;

} // namespace scanner