#pragma once

#include "championship.hpp"
#include "club.hpp"
#include "club_season.hpp"

#include <string>
#include <vector>

namespace football
{

class season final : public mysql::table
{
  public:
    season(championship &championship, const std::string &year)
        : table("seasons", "championship", "year"), _id(""), _championship(championship), _year(year), _clubs{}
    {
        if (start("championship", _championship.id(), "year", _year))
        {
            _id = get("id");
            _year = get("year");

            start_related();
        }
    }

    season(pchampionship &championship, const std::string &year) : season(*championship, year)
    {
    }

    season(const std::string &id, championship &championship)
        : table("seasons", "championship", "year"), _id(id), _championship(championship), _year(""), _clubs{}
    {
        if (start("id", _id))
        {
            _id = get("id");
            _year = get("year");

            start_related();
        }
    }

    season(const std::string &id)
        : table("seasons", "championship", "year"), _id(id), _championship{}, _year(""), _clubs{}
    {
        if (start("id", _id))
        {
            _id = get("id");
            _year = get("year");

            start_related();
        }
    }

    auto id() const -> std::string override
    {
        return _id;
    }
    auto year() const -> std::string
    {
        return _year;
    }
    auto championshp() const -> const championship &
    {
        return _championship;
    }

    void add_club(club &cl)
    {
        if (auto it =
                std::find_if(_clubs.begin(), _clubs.end(),
                             [&](const club &c) { return (c.name() == cl.name() && c.country() == cl.country()); });
            it == _clubs.end())
        {
            _clubs.emplace_back(cl);
        }
    }
    void add_club(pclub &cl)
    {
        if (auto it =
                std::find_if(_clubs.begin(), _clubs.end(),
                             [&](const club &c) { return (c.name() == cl->name() && c.country() == cl->country()); });
            it == _clubs.end())
        {
            _clubs.emplace_back(*cl);
        }
    }
    const football::club &club_at(std::size_t index) const
    {
        return _clubs.at(index);
    }
    std::size_t club_count()
    {
        return _clubs.size();
    }
    [[nodiscard]] bool remove_club(const club &cl)
    {
        if (auto it =
                std::find_if(_clubs.begin(), _clubs.end(),
                             [&](const club &c) { return (c.name() == cl.name() && c.country() == cl.country()); });
            it != _clubs.end())
        {
            _clubs.erase(it);
            return true;
        }
        return false;
    }

    bool empty() override
    {
        return (_id.empty() && _year.empty() && _clubs.empty());
    }

  private:
    std::string _id;
    const football::championship &_championship;
    std::string _year;
    std::vector<club> _clubs;
    uint8_t _spots{};

    void populate() override
    {
        set("id", _id);
        set("championship", _championship.id());
        set("year", _year);
    }

    void reset() override
    {
        _id = get("id");
        _year = get("year");
    }

    virtual auto start_related() -> void override
    {
        club_season cs{_id};
        std::vector<std::string> ids{};

        while (cs.next())
        {
            auto club_id = cs.get_value("club");
            ids.push_back(club_id);
        }

        for (auto const &club_id : ids)
        {
            _clubs.emplace_back(club_id);
        }
    }

    auto save_related() -> bool override
    {
        bool result{true};
        for (auto const &club : _clubs)
        {
            club_season cs{club.id(), _id};
            result = cs.save();
        }
        return result;
    }
};

using pseason = std::unique_ptr<season>;

} // namespace football