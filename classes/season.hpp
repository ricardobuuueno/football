#pragma once

#include "championship.hpp"
#include "club.hpp"

#include <string>
#include <vector>

namespace football
{

	class season final
	{
	public:
		championship championship;
		std::string year;
		std::vector<club> clubs;

	};

}