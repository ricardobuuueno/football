#pragma once

#include "../db/mysql.hpp"

namespace football
{

class club_season final : public mysql::table
{
  public:
    club_season(const std::string &club_id, const std::string &season_id)
        : table("clubs_seasons", "club", "season"), _club_id(club_id), _season_id(season_id)
    {
        populate();
    }

    club_season(const std::string &season_id) : table("clubs_seasons", "season"), _season_id(season_id)
    {
        start_list("season", _season_id);
    }

    bool empty() override
    {
        return false;
    }

  private:
    std::string _club_id;
    std::string _season_id;

    void populate() override
    {
        set("club", _club_id);
        set("season", _season_id);
    }

    void reset() override
    {
        _club_id = get("club");
        _season_id = get("season");
    }
};

using pclub_season = std::unique_ptr<club_season>;

} // namespace football