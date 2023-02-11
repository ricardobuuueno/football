#pragma once

#include <map>

#include "club.hpp"
#include "player.hpp"
#include "manager.hpp"

namespace football
{

	class team final
	{
	public:
		club club;
		std::map<int, player> players;
		manager manager;

	};

}