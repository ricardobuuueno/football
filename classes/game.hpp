#pragma once

#include "team.hpp"
#include "referee.hpp"
#include "season.hpp"
#include "score.hpp"
#include "stadium.hpp"

namespace football
{

	class game final
	{
	public:
		season _season;
		team _teamA;
		team _teamB;
		referee _referee;
		score _score;
		stadium _stadium;
		score _penalties;

	};

}