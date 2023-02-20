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
    club(const std::string &club_name, const std::string &country_code)
        : table("clubs", "name", "country"), _id(""), _name(club_name), _country(country_code), _players{}
    {
        if (start("name", _name, "country", _country))
        {
            _id = get("id");
        }
    }

    club() : club("", "")
    {
    }

    club(const std::string &id) : table("clubs", "name", "country"), _id(id), _name(""), _country(""), _players{}
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

    club &operator=(football::club &&other)
    {
        _id = std::move(other._id);
        _name = std::move(other._name);
        _country = std::move(other._country);
        _players = std::move(other._players);
        return *this;
    }

    bool operator==(const club &other) const
    {
        return (_name == other.name() && _country == other.country());
    }

    std::string id() const
    {
        return _id;
    }
    std::string name() const
    {
        return _name;
    }
    std::string country() const
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

    bool empty() override
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
