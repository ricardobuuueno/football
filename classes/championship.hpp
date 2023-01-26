#pragma once

#include <string>
#include <vector>

#include "club.hpp"

namespace football
{

	class championship final
	{
	public:
		std::string name;
		std::vector<club> clubs;

	};

}