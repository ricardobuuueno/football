#include "pch.h"
#include "../classes/club.hpp"
TEST(Clubs, club_name) 
{
	football::club sp{ "São Paulo" };
	EXPECT_EQ(sp.name(), "São Paulo");

}