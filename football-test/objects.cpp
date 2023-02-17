#include "pch.h"

TEST(Objects, club) 
{
	football::club club1{};
	EXPECT_EQ(club1.empty(), true);
	EXPECT_EQ(club1.sql_count(), 0);

	std::string NAME = "Club";
	club1.name(NAME);
	auto saved = club1.save();

	EXPECT_EQ(saved, true);
	EXPECT_NE(club1.id(), "");
	EXPECT_EQ(club1.name(), NAME);
	EXPECT_EQ(club1.empty(), false);
	EXPECT_NE(club1.sql_count(), 0);

    std::string NAME2 = "Club2";
	football::club club2{club1};
	
	EXPECT_EQ(club2, club1);
	EXPECT_EQ(club2.empty(), false);
	EXPECT_EQ(club2.sql_count(), 0);
    
    club2.name(NAME2);
    auto saved2 = club2.save();
    EXPECT_EQ(saved2, true);
    EXPECT_NE(club2.id(), "");
	EXPECT_NE(club2.sql_count(), 0);
    EXPECT_EQ(club2.id(), club1.id());

    std::string NAME3 = "Club3";
	football::club club3 = club1;

	EXPECT_EQ(club3, club1);
	EXPECT_EQ(club3.empty(), false);
	EXPECT_EQ(club3.sql_count(), 0);

    club3.name(NAME3);
    auto saved3 = club3.save();
    EXPECT_EQ(saved3, true);
    EXPECT_NE(club3.id(), "");
	EXPECT_NE(club3.sql_count(), 0);
    EXPECT_EQ(club3.id(), club1.id());
    EXPECT_EQ(club3.id(), club2.id());

    football::club club4{std::move(club1)};

    EXPECT_EQ(club1.empty(), true);

    auto saved4 = club4.save();
    EXPECT_EQ(saved4, true);
    EXPECT_EQ(club4.id(), club2.id());
    EXPECT_EQ(club4.id(), club3.id());

    football::club club5 = std::move(club2);

    EXPECT_EQ(club2.empty(), true);
    
    auto saved5 = club5.save();
    EXPECT_EQ(saved5, true);
    EXPECT_EQ(club5.id(), club3.id());
    EXPECT_EQ(club5.id(), club4.id());

    auto removed = club5.remove();
    EXPECT_EQ(removed, true);
    EXPECT_EQ(club5.empty(), true);

}
