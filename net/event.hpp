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
    uint64_t id{0};
};

struct new_championship
{
    std::array<char, 100> name;
    std::array<char, 2> country;
};

} // namespace net