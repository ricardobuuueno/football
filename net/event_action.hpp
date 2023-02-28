#pragma once

#include "event.hpp"

namespace net
{

auto event_action(const new_championship &champ) -> response;
auto event_action(const new_club &club) -> response;

} // namespace net