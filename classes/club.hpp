#pragma once

#include <string>
#include <vector>

#include "../db/mysql.hpp"
#include "player.hpp"

namespace football
{

class club final : public mysql::table
{
  public:
    club(std::string club_name, std::string country_code)
        : table("clubs", "name", "country"), _id(""), _name(std::move(club_name)),
          _country(std::move(country_code)), _players{}
    {
        if (start("name", _name, "country", _country))
        {
            _id = get("id");
        }
    }

    club() : club("", "")
    {
    }

    club(std::string id) : table("clubs", "name", "country"), _id(std::move(id)), _name(""), _country(""), _players{}
    {
        if (start("id", _id))
        {
            _name = get("name");
            _country = get("country");
        }
    }

    club(const football::club &other) : table(other)
    {
        _id = other.id();
        _name = other.name();
        _country = other.country();
    }

    club(football::club &&other) : table(other)
    {
        _id = std::move(other._id);
        _name = std::move(other._name);
        _country = std::move(other._country);
        _players = std::move(other._players);
    }

    auto operator=(football::club &&other) -> club &
    {
        _id = std::move(other._id);
        _name = std::move(other._name);
        _country = std::move(other._country);
        _players = std::move(other._players);
        return *this;
    }

    auto operator==(const club &other) const -> bool
    {
        return (_id == other.id() && _name == other.name() && _country == other.country());
    }

    [[nodiscard]] auto id() const -> std::string override
    {
        return _id;
    }
    [[nodiscard]] auto name() const -> std::string
    {
        return _name;
    }
    [[nodiscard]] auto country() const -> std::string
    {
        return _country;
    }

    void name(const std::string &club_name)
    {
        _name = club_name;
    }
    void country(const std::string &country_code)
    {
        _country = country_code;
    }

    auto empty() -> bool override
    {
        return (_id.empty() && _name.empty() && _country.empty() && _players.empty());
    }

  private:
    std::string _id;
    std::string _name;
    std::string _country;
    std::vector<player> _players;

    void populate() override
    {
        set("id", _id);
        set("name", _name);
        set("country", _country);
    }

    void reset() override
    {
        _id = get("id");
        _name = get("name");
        _country = get("country");
    }
};

using pclub = std::unique_ptr<club>;

} // namespace football
