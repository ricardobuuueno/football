#include "pch.h"
#include "../classes/club.hpp"
TEST(Clubs, club_name) 
{
	football::club sp{ "S�o Paulo" };
	EXPECT_EQ(sp.name(), "S�o Paulo");

}