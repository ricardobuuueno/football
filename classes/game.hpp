#pragma once

#include "team.hpp"
#include "referee.hpp"
#include "championship.hpp"
#include "score.hpp"

namespace football
{

	class game final
	{
	public:
		championship championship;
		team teamA;
		team teamB;
		referee referee;
		score score;

	};

}