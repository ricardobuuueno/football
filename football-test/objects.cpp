#include "pch.h"

/* TEST(Objects, club)
{
    football::club club1{};
    EXPECT_EQ(club1.empty(), true);
    EXPECT_EQ(club1.sql_count(), 0);

    std::string NAME = "Club";
    std::string COUNTRY = "BR";
    club1.name(NAME);
    club1.country(COUNTRY);
    auto saved = club1.save();

    EXPECT_EQ(saved, true);
    EXPECT_NE(club1.id(), "");
    EXPECT_EQ(club1.name(), NAME);
    EXPECT_EQ(club1.country(), COUNTRY);
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

TEST(Objects, championship)
{
    using football::championship;

    const std::string CHAMPIONSHIP{"Championship"};
    const std::string COUNTRY{"GB"};

    auto cs = championship{};
    EXPECT_EQ(cs.empty(), true);
    EXPECT_EQ(cs.sql_count(), 0);

    cs.name(CHAMPIONSHIP);
    cs.country(COUNTRY);

    auto saved = cs.save();
    std::string id = cs.id();

    EXPECT_EQ(saved, true);
    EXPECT_EQ(cs.empty(), false);
    EXPECT_NE(cs.sql_count(), 0);
    EXPECT_EQ(cs.name(), CHAMPIONSHIP);
    EXPECT_EQ(cs.country(), COUNTRY);

    auto cs2 = championship{CHAMPIONSHIP, COUNTRY};
    EXPECT_EQ(cs.empty(), false);
    EXPECT_NE(cs.sql_count(), 0);
    EXPECT_EQ(cs.name(), cs2.name());
    EXPECT_EQ(cs.country(), cs2.country());
    EXPECT_EQ(cs.id(), cs2.id());

    saved = cs2.save();
    EXPECT_EQ(saved, true);
    EXPECT_EQ(id, cs2.id());

    championship copy = cs2;
    EXPECT_EQ(cs2, copy);
    EXPECT_EQ(cs, copy);
    EXPECT_EQ(copy.sql_count(), 0);

    championship copy2(cs2);
    EXPECT_EQ(cs2, copy2);
    EXPECT_EQ(cs, copy2);
    EXPECT_EQ(copy2.sql_count(), 0);

    auto removed = copy2.remove();
    EXPECT_EQ(removed, true);

    auto cs3 = championship{CHAMPIONSHIP, COUNTRY};
    EXPECT_EQ(cs3.id(), "");
}

TEST(Objects, season)
{
    using football::championship;
    using football::club;
    using football::New;
    using football::season;

    const std::string CHAMPIONSHIP{"Championship"};
    const std::string COUNTRY{"GB"};
    const std::string YEAR{"2024"};
    const std::string CLUB{"THEFC"};

    auto cs = New<championship>(CHAMPIONSHIP, COUNTRY);
    auto saved = cs->save();
    EXPECT_EQ(saved, true);

    auto ss = season{cs, YEAR};
    saved = ss.save();
    EXPECT_EQ(saved, true);

    auto cl = club{CLUB, COUNTRY};
    saved = cl.save();
    EXPECT_EQ(saved, true);

    ss.add_club(cl);
    ss.add_club(cl);
    saved = ss.save();
    EXPECT_EQ(saved, true);
    EXPECT_EQ(ss.club_count(), 1);

    auto removed = ss.remove();
    EXPECT_EQ(removed, true);

    removed = cs->remove();
    EXPECT_EQ(removed, true);

    removed = cl.remove();
    EXPECT_EQ(removed, true);
} */