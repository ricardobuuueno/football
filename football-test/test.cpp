#include "pch.h"

class FootballEnvironment : public ::testing::Environment {
	public:
		FootballEnvironment(const std::string& conf_file) 
			: config_file(conf_file) {}

  		~FootballEnvironment() override {}

		// Override this to define how to set up the environment.
		void SetUp() override 
		{
			football::init(config_file);
		}

		// Override this to define how to tear down the environment.
		void TearDown() override {}
	
	private:
		const std::string config_file;

};

int main(int argc, char **argv)
{
	std::string config_filename{argv[0]};
	config_filename += ".toml";

	FootballEnvironment* env = new FootballEnvironment(config_filename); 
	::testing::AddGlobalTestEnvironment(env);	

	::testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}

TEST(Championships, new_championship_added) 
{
	const std::string CHAMPIONSHIP{"Championship"};
	const std::string COUNTRY{"GB"};

	football::championship cs{ CHAMPIONSHIP, COUNTRY };
	EXPECT_EQ(cs.id(), "");

	auto saved = cs.save();
	EXPECT_EQ(saved, true);

	EXPECT_NE(cs.id(), "");
	EXPECT_EQ(cs.name(), CHAMPIONSHIP);
	EXPECT_EQ(cs.country(), COUNTRY);

	auto removed = cs.remove();
	EXPECT_EQ(removed, true);

	EXPECT_EQ(cs.id(), "");
	EXPECT_EQ(cs.name(), "");
	EXPECT_EQ(cs.country(), "");

}

TEST(Championships, start_new_season)
{
	const char CLUB_COUNT{20};
	const std::string YEAR{"2023/2024"};

	football::championship championship{ "Championship", "GB" };
	bool saved = championship.save();
	EXPECT_EQ(saved, true);	
	EXPECT_NE(championship.id(), "");	
	
	football::season season{ championship, YEAR};
	EXPECT_EQ(season.id(), "");
	saved = season.save();
	EXPECT_EQ(saved, true);	
	EXPECT_NE(season.id(), "");	
	EXPECT_EQ(season.championship().id(), championship.id());
	EXPECT_EQ(season.year(), YEAR);

	std::vector<football::club> clubs{};
	clubs.reserve(CLUB_COUNT);

	for (char i{0}; i < CLUB_COUNT; ++i) 
	{
		std::string club_name{(char)('A' + i)};
		football::club club{club_name};
		EXPECT_EQ(club.id(), "");	
		saved = club.save();
		EXPECT_EQ(saved, true);	
		clubs.push_back(club);
	}

	EXPECT_EQ(clubs.size(), CLUB_COUNT);
	for (auto const& club : clubs) {
		season.add_club(club);
		EXPECT_NE(club.id(), "");	
	}

	EXPECT_EQ(clubs.size(), CLUB_COUNT);
	EXPECT_EQ(season.club_count(), CLUB_COUNT);

	bool removed{};
	for (char i{CLUB_COUNT - 1}; i >= 0; --i) 
	{
		football::club club = season.club_at(i);
		
		EXPECT_NE(club.id(), "");
		season.remove_club(club);
		
		removed = club.remove();
		EXPECT_EQ(removed, true);	
	}

	EXPECT_EQ(season.club_count(), 0);

	removed = season.remove();
	EXPECT_EQ(removed, true);	

	removed = championship.remove();
	EXPECT_EQ(removed, true);	

}