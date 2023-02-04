#pragma once

#include "team.hpp"
#include "referee.hpp"
#include "championship.hpp"
#include "score.hpp"
#include "stadium.hpp"

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
		stadium stadium;

	};

}