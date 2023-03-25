#pragma once

#include "../db/mysql.hpp"
#include "../pub/publisher_base.hpp"
#include "../util/json.hpp"

#include "task_result.hpp"

#include <memory>

namespace scanner
{

enum class task_type : uint8_t
{
    none,
    new_championship
};

enum class task_status : uint8_t
{
    none,
    new_task,
    scheduled,
    running,
    finished,
    error
};

class task_base : public mysql::table
{
  public:
    task_base(const std::string &id, pub::publisher pub)
        : table("tasks", "id"), _id(id), _type(task_type::none), _status(task_status::none), _publisher(pub),
          _results("")
    {
        if (start("id", _id))
        {
            _type = static_cast<task_type>(get_int("type"));
            _status = static_cast<task_status>(get_int("status"));
            _publisher = static_cast<pub::publisher>(get_int("publisher"));
            _results = get("results");
            read_properties(get("properties"));
        }
    }

    explicit task_base(const std::string &id) : task_base(id, pub::publisher::none)
    {
    }

    task_base() : task_base("")
    {
        populate();
    }

    task_base(const task_base &other) : table(other)
    {
        _id = other._id;
        _type = other._type;
        _status = other._status;
        _publisher = other._publisher;
        _properties = other._properties;
        _results = other._results;
    }

    task_base(task_base &&other) : table(other)
    {
        _id = other._id;
        _type = other._type;
        _status = other._status;
        _publisher = other._publisher;
        _properties = other._properties;
        _results = other._results;
    }

    task_base &operator=(const task_base &other)
    {
        _id = other._id;
        _type = other._type;
        _status = other._status;
        _publisher = other._publisher;
        _properties = other._properties;
        _results = other._results;
        return *this;
    }

    task_base &operator=(task_base &&other)
    {
        _id = other._id;
        _type = other._type;
        _status = other._status;
        _publisher = other._publisher;
        _properties = other._properties;
        _results = other._results;
        return *this;
    }

    auto operator==(const task_base &other) const -> bool
    {
        return (_id == other._id && _type == other._type && _status == other._status && _publisher == other._publisher);
    }

    [[nodiscard]] auto id() const -> std::string override
    {
        return _id;
    }
    [[nodiscard]] auto type() const -> task_type
    {
        return _type;
    }
    [[nodiscard]] auto status() const -> task_status
    {
        return _status;
    }

    [[nodiscard]] auto publisher() const -> pub::publisher
    {
        return _publisher;
    }

    [[nodiscard]] auto results() const -> std::string
    {
        return _results;
    }

    auto type(const task_type type) -> void
    {
        _type = type;
    }
    auto status(const task_status status) -> void
    {
        _status = status;
    }

    auto publisher(const pub::publisher publisher) -> void
    {
        _publisher = publisher;
    }

    auto results(const std::string results) -> void
    {
        _results = results;
    }

    [[nodiscard]] auto empty() -> bool override
    {
        return (_id.empty() && _type == task_type::none && _status == task_status::none &&
                _publisher == pub::publisher::none);
    }

    virtual auto run() -> std::tuple<bool, std::string, ptask_result> = 0;

  protected:
    auto set_property(std::string key, std::string value) -> void
    {
        _properties[key] = value;
    }

    auto get_property(std::string key) -> std::string
    {
        return _properties[key];
    }

  private:
    std::string _id;
    task_type _type;
    task_status _status;
    pub::publisher _publisher;
    std::string _results;

    std::map<std::string, std::string> _properties{};

    virtual auto dump_properties() -> std::string
    {
        nlohmann::json j;
        for (auto const &[key, value] : _properties)
        {
            j[key] = value;
        }
        return j.dump();
    }

    virtual auto read_properties(const std::string &props) -> void
    {
        auto j = nlohmann::json::parse(props);
        for (auto &[key, value] : j.items())
        {
            set_property(key, value);
        }
    }

    void populate() override
    {
        set("id", _id);
        set("type", static_cast<uint64_t>(_type));
        set("status", static_cast<uint64_t>(_status));
        set("publisher", static_cast<uint64_t>(_publisher));
        set("properties", dump_properties());
        set("results", _results);
    }

    void reset() override
    {
        _id = get("id");
        _type = static_cast<task_type>(get_int("type"));
        _status = static_cast<task_status>(get_int("status"));
        _publisher = static_cast<pub::publisher>(get_int("publisher"));
        _results = get("results");
        read_properties(get("properties"));
    }
};

using ptask_base = std::unique_ptr<task_base>;

} // namespace scanner