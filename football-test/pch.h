//
// pch.h
//

#pragma once

#include "../classes/championship.hpp"
#include "../classes/club.hpp"
#include "../classes/club_season.hpp"
#include "../classes/season.hpp"
#include "../net/event.hpp"
#include "../net/football_client.hpp"
#include "../net/football_server.hpp"
#include "../util/globals.hpp"
#include "gtest/gtest.h"

#include <future>
#include <thread>

namespace test
{

auto sleep(int secs) -> void;

} // namespace test
