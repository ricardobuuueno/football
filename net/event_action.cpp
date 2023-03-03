#include "event_action.hpp"

#include "../classes/championship.hpp"
#include "../classes/club.hpp"
#include "../classes/season.hpp"

namespace net
{

auto event_action(const new_championship &champ) -> response
{
    std::string name(champ.name.begin(), std::find(champ.name.begin(), champ.name.end(), '\0'));
    std::string country(champ.country.begin(), champ.country.end());
    football::championship cs{name, country};
    bool success = cs.save();

    return {cs.id_int(), success};
}

auto event_action(const new_club &club) -> response
{
    std::string name(club.name.begin(), std::find(club.name.begin(), club.name.end(), '\0'));
    std::string country(club.country.begin(), club.country.end());
    football::club cl{name, country};
    bool success = cl.save();

    return {cl.id_int(), success};
}

auto event_action(const new_season &season) -> response
{
    std::string cs_id = std::to_string(season.championship);
    std::string year(season.year.begin(), std::find(season.year.begin(), season.year.end(), '\0'));
    football::championship cs{cs_id};
    football::season ss{cs, year};
    bool success = ss.save();

    return {ss.id_int(), success};
}

} // namespace net