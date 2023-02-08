
#include <iostream>
#include "classes/club.hpp"

int main()
{
	using namespace football;

	club sp{"São Paulo"};

    std::cout << "Club: " << sp.id() << " " << sp.name() << '\n';
}

