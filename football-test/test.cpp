#include "pch.h"

#include "../globals.hpp"
#include "../classes/championship.hpp"
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

TEST(Championships, new_championship_added) 
{
	football::championship cs{ "Championship", "GB" };
	EXPECT_EQ(cs.id(), "");

	auto saved = cs.save();
	EXPECT_EQ(saved, true);

	EXPECT_NE(cs.id(), "");
	EXPECT_EQ(cs.name(), "Championship");
	EXPECT_EQ(cs.country(), "GB");

	auto removed = cs.remove();
	EXPECT_EQ(removed, true);

	EXPECT_EQ(cs.id(), "");
	EXPECT_EQ(cs.name(), "");
	EXPECT_EQ(cs.country(), "");

}

