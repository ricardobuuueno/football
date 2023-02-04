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
		std::string year;
		championship championship;
		std::vector<club> clubs;

	};

}