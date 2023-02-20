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
    season(pchampionship &championship, const std::string &year)
        : table("seasons", "championship", "year"), _id(""), _championship(*championship), _year(year), _clubs{}
    {
        if (start("championship", _championship.id(), "year", _year))
        {
            _id = get("id");
            _year = get("year");

            start_related();
        }
    }

    std::string id() const
    {
        return _id;
    }
    std::string year() const
    {
        return _year;
    }
    const championship &championshp() const
    {
        return _championship;
    }

    void add_club(club &club)
    {
        _clubs.emplace_back(club);
    }
    void add_club(pclub &club)
    {
        _clubs.emplace_back(*club);
    }
    const football::club &club_at(std::size_t index) const
    {
        return _clubs.at(index);
    }
    std::size_t club_count()
    {
        return _clubs.size();
    }
    [[nodiscard]] bool remove_club(const club &club)
    {
        if (auto it = std::find(_clubs.begin(), _clubs.end(), club); it != _clubs.end())
        {
            _clubs.erase(it);
            return true;
        }
        return false;
    }

    bool empty() override
    {
        return (_id.empty() && _championship.empty() && _year.empty() && _clubs.empty());
    }

  private:
    std::string _id;
    football::championship &_championship;
    std::string _year;
    std::vector<club> _clubs;

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