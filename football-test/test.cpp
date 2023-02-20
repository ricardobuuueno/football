#include "pch.h"

class FootballEnvironment : public ::testing::Environment
{
  public:
    FootballEnvironment(const std::string &conf_file) : config_file(conf_file)
    {
    }

    ~FootballEnvironment() override
    {
    }

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        football::init(config_file);
    }

    // Override this to define how to tear down the environment.
    void TearDown() override
    {
    }

  private:
    const std::string config_file;
};

int main(int argc, char **argv)
{
    std::string config_filename{argv[0]};
    config_filename += ".toml";

    FootballEnvironment *env = new FootballEnvironment(config_filename);
    ::testing::AddGlobalTestEnvironment(env);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Championships, new_championship_season_and_club)
{
    using football::championship;
    using football::club;
    using football::New;
    using football::season;

    const std::string CHAMPIONSHIP{"Championship"};
    const std::string COUNTRY{"GB"};
    const std::string YEAR{"2023/2024"};
    const std::string CLUB{"THEFC"};

    auto cs = New<championship>(CHAMPIONSHIP, COUNTRY);
    auto saved = cs->save();
    EXPECT_EQ(saved, true);

    auto ss = New<season>(cs, YEAR);
    saved = ss->save();
    EXPECT_EQ(saved, true);

    auto cl = New<club>(CLUB, COUNTRY);
    saved = cl->save();
    EXPECT_EQ(saved, true);

    ss->add_club(cl);
    EXPECT_NE(cl, nullptr);

    saved = ss->save();
    EXPECT_EQ(saved, true);

    auto cs2 = New<championship>(CHAMPIONSHIP, COUNTRY);
    EXPECT_EQ(cs2->id(), cs->id());

    auto ss2 = New<season>(cs, YEAR);
    EXPECT_EQ(ss2->id(), ss->id());
    EXPECT_EQ(ss2->club_count(), 1);

    auto &cl2 = ss2->club_at(0);
    EXPECT_EQ(cl2.name(), CLUB);
    EXPECT_EQ(cl2.country(), COUNTRY);

    ss2->remove_club(cl2);
    EXPECT_EQ(ss2->club_count(), 0);

    saved = ss2->save();
    EXPECT_EQ(saved, true);
    EXPECT_EQ(ss2->club_count(), 0);

    auto removed = cl->remove();
    EXPECT_EQ(removed, true);

    removed = ss->remove();
    EXPECT_EQ(removed, true);

    removed = cs->remove();
    EXPECT_EQ(removed, true);
}

TEST(Championships, new_championship_added)
{
    using football::championship;
    using football::New;

    const std::string CHAMPIONSHIP{"Championship"};
    const std::string COUNTRY{"GB"};

    auto cs = New<championship>(CHAMPIONSHIP, COUNTRY);
    EXPECT_EQ(cs->id(), "");

    auto saved = cs->save();
    EXPECT_EQ(saved, true);

    EXPECT_NE(cs->id(), "");
    EXPECT_EQ(cs->name(), CHAMPIONSHIP);
    EXPECT_EQ(cs->country(), COUNTRY);

    auto removed = cs->remove();
    EXPECT_EQ(removed, true);

    EXPECT_EQ(cs->id(), "");
    EXPECT_EQ(cs->name(), "");
    EXPECT_EQ(cs->country(), "");
}

TEST(Championships, start_new_season)
{
    using football::championship;
    using football::club;
    using football::New;
    using football::season;

    const char CLUB_COUNT{20};
    const std::string COUNTRY{"AR"};
    const std::string YEAR{"2023/2024"};

    PLOG_DEBUG << "[TEST] Creating championship";

    auto cs = New<championship>("Championship", COUNTRY);
    bool saved = cs->save();
    EXPECT_EQ(saved, true);
    EXPECT_NE(cs->id(), "");

    PLOG_DEBUG << "[TEST] Creating the season";

    auto ss = New<season>(cs, YEAR);
    EXPECT_EQ(ss->id(), "");
    saved = ss->save();
    EXPECT_EQ(saved, true);
    EXPECT_NE(ss->id(), "");
    EXPECT_EQ(ss->championshp().id(), cs->id());
    EXPECT_EQ(ss->year(), YEAR);

    std::vector<football::club> clubs{};
    clubs.reserve(CLUB_COUNT);

    PLOG_DEBUG << "[TEST] Creating the list of clubs";

    for (char i{0}; i < CLUB_COUNT; ++i)
    {
        std::string club_name{(char)('A' + i)};
        auto cl = club{club_name, COUNTRY};
        EXPECT_EQ(cl.id(), "");
        saved = cl.save();
        EXPECT_EQ(saved, true);
        clubs.push_back(cl);
    }

    PLOG_DEBUG << "[TEST] Adding clubs to the season object";

    EXPECT_EQ(clubs.size(), CLUB_COUNT);
    for (auto &cl : clubs)
    {
        ss->add_club(cl);
        EXPECT_NE(cl.id(), "");
    }

    EXPECT_EQ(clubs.size(), CLUB_COUNT);
    EXPECT_EQ(ss->club_count(), CLUB_COUNT);

    PLOG_DEBUG << "[TEST] Removing clubs from the season object";

    bool removed{};
    for (char i{CLUB_COUNT - 1}; i >= 0; --i)
    {
        auto cl = ss->club_at(i);

        EXPECT_NE(cl.id(), "");
        ss->remove_club(cl);

        removed = cl.remove();
        EXPECT_EQ(removed, true);
    }

    EXPECT_EQ(ss->club_count(), 0);

    removed = ss->remove();
    EXPECT_EQ(removed, true);

    removed = cs->remove();
    EXPECT_EQ(removed, true);
}