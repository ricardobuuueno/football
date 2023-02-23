#pragma once

#include "common.hpp"

namespace net
{

enum class event_type : uint32_t
{
    new_championship,
    new_club,
    new_season,
    new_game,
    game_start,
    game_ended,
    goal_scored
};

} // namespace net