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
    bool success{false};
};

struct new_championship
{
    std::array<wchar_t, 100> name;
    std::array<wchar_t, 2> country;
    event_type type{event_type::new_championship};
};

struct new_club
{
    std::array<wchar_t, 100> name;
    std::array<wchar_t, 2> country;
    event_type type{event_type::new_club};
};

} // namespace net