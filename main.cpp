
#include <iostream>
#include "classes/club.hpp"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"

int main()
{
	using namespace football;

	plog::init(plog::debug, "log.csv", 5000, 3);

	club sp{"São Paulo"};

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';
}

