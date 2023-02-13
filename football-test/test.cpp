#include "pch.h"

#include "../globals.hpp"
#include "../classes/club.hpp"

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

TEST(Clubs, club_name) 
{
	football::club sp{ "São Paulo" };
	EXPECT_EQ(sp.name(), "São Paulo");
}

