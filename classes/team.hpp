#pragma once

#include <vector>

#include "club.hpp"
#include "player.hpp"

namespace football
{

	class team final
	{
	public:
		club club;
		std::vector<player> players;

	};

}