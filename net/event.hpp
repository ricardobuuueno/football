#pragma once

#include "common.hpp"

namespace net
{

enum class event_type : uint32_t
{
    accept,
    ping,
    new_championship,
    new_club,
    new_season,
    new_game,
    game_start,
    game_ended,
    goal_scored
};

struct response
{
    std::string id;
};

struct new_championship
{
    std::string name;
    std::string country;
};

} // namespace net