#pragma once

#include "player.hpp"
#include "club.hpp"

namespace football
{

	class goal final
	{
	public:
		club club;
		player player;
		int period;
		int minute;

	};

}