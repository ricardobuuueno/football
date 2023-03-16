#pragma once

#include "../db/mysql.hpp"

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
    task_base(const std::string &id) : table("tasks", "id"), _id(id), _type(task_type::none), _status(task_status::none)
    {
        if (start("id", _id))
        {
            _type = static_cast<task_type>(get_int("type"));
            _status = static_cast<task_status>(get_int("status"));
        }
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
    }

    task_base &operator=(const task_base &other)
    {
        _id = other._id;
        _type = other._type;
        _status = other._status;
        return *this;
    }

    auto operator==(const task_base &other) const -> bool
    {
        return (_id == other._id && _type == other._type && _status == other._status);
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

    auto type(const task_type type) -> void
    {
        _type = type;
    }
    auto status(const task_status status) -> void
    {
        _status = status;
    }

    [[nodiscard]] auto empty() -> bool override
    {
        return (_id.empty() && _type == task_type::none && _status == task_status::none);
    }

  private:
    std::string _id;
    task_type _type;
    task_status _status;

    void populate() override
    {
        set("id", _id);
        set("type", static_cast<uint64_t>(_type));
        set("status", static_cast<uint64_t>(_status));
    }

    void reset() override
    {
        _id = get("id");
        _type = static_cast<task_type>(get_int("type"));
        _status = static_cast<task_status>(get_int("status"));
    }
};

} // namespace scanner