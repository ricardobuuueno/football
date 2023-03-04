#include "pch.h"

const std::string HOST{"127.0.0.1"};
const uint16_t PORT{60000};

TEST(Football_Server, start_server_thread)
{
    net::football_client client{};
    client.connect(HOST, PORT);
    EXPECT_EQ(client.is_connected(), true);

    client.ping_server();

    auto accepted{false};
    auto answered{false};

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::accept:
                    accepted = true;
                    break;
                case net::event_type::ping:
                    answered = true;
                    break;
                }
            }

            if (accepted && answered)
            {
                break;
            }
        }
    }

    EXPECT_EQ(accepted, true);
    EXPECT_EQ(answered, true);
}

TEST(Football_Server, new_championship_event)
{
    const std::string CHAMP{"Prêmier Léague"};
    const std::string COUNTRY{"EN"};

    net::football_client client{};
    client.connect(HOST, PORT);
    EXPECT_EQ(client.is_connected(), true);

    auto new_champ = net::new_championship{};
    std::copy(CHAMP.begin(), CHAMP.end(), new_champ.name.data());
    std::copy(COUNTRY.begin(), COUNTRY.end(), new_champ.country.data());
    net::message<net::event_type> msg;
    msg.header.id = net::event_type::new_championship;
    msg << new_champ;

    client.send(msg);

    auto accepted{false};
    net::response res{};

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::accept:
                    accepted = true;
                    break;
                case net::event_type::new_championship:
                    msg >> res;
                    std::cout << res.id << '\n';
                    break;
                }
            }

            if (accepted && res.id)
            {
                break;
            }
        }
    }

    EXPECT_EQ(accepted, true);
    EXPECT_NE(res.id, 0);

    std::string str_id = std::to_string(res.id);
    football::championship cs{str_id};
    EXPECT_EQ(cs.id(), str_id);
    EXPECT_EQ(cs.name(), CHAMP);
    EXPECT_EQ(cs.country(), COUNTRY);

    auto removed = cs.remove();
    EXPECT_EQ(removed, true);

    football::championship cs2{str_id};
    EXPECT_EQ(cs2.found(), false);
}

TEST(Football_Server, new_club_event)
{
    const std::string CHAMP{"Prêmier Léague"};
    const std::string CLUB{"Lêtégão 2º Football Club"};
    const std::string COUNTRY{"EN"};

    net::football_client client{};
    client.connect(HOST, PORT);
    EXPECT_EQ(client.is_connected(), true);

    auto new_champ = net::new_championship{};
    std::copy(CHAMP.begin(), CHAMP.end(), new_champ.name.data());
    std::copy(COUNTRY.begin(), COUNTRY.end(), new_champ.country.data());
    net::message<net::event_type> msg;
    msg.header.id = net::event_type::new_championship;
    msg << new_champ;

    client.send(msg);

    auto new_club = net::new_club{};
    std::copy(CLUB.begin(), CLUB.end(), new_club.name.data());
    std::copy(COUNTRY.begin(), COUNTRY.end(), new_club.country.data());
    net::message<net::event_type> msg_club;
    msg_club.header.id = net::event_type::new_club;
    msg_club << new_club;

    client.send(msg_club);

    auto accepted{false};
    net::response champ_res{};
    net::response club_res{};

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::accept:
                    accepted = true;
                    break;
                case net::event_type::new_championship:
                    msg >> champ_res;
                    break;
                case net::event_type::new_club:
                    msg >> club_res;
                    break;
                }
            }

            if (accepted && champ_res.id && club_res.id)
            {
                break;
            }
        }
    }

    EXPECT_EQ(accepted, true);
    EXPECT_NE(champ_res.id, 0);

    std::string cs_id = std::to_string(champ_res.id);
    football::championship cs{cs_id};
    EXPECT_EQ(cs.id(), cs_id);
    EXPECT_EQ(cs.name(), CHAMP);
    EXPECT_EQ(cs.country(), COUNTRY);

    std::string cl_id = std::to_string(club_res.id);
    football::club cl{cl_id};
    EXPECT_EQ(cl.id(), cl_id);
    EXPECT_EQ(cl.name(), CLUB);
    EXPECT_EQ(cl.country(), COUNTRY);

    auto removed = cl.remove();
    EXPECT_EQ(removed, true);

    removed = cs.remove();
    EXPECT_EQ(removed, true);

    football::championship cs2{cs_id};
    EXPECT_EQ(cs2.found(), false);

    football::club cl2{cl_id};
    EXPECT_EQ(cl2.found(), false);
}

TEST(Football_Server, new_season_event)
{
    const std::string CHAMP{"Prêmier Léague"};
    const std::string CLUB{"Lêtégão 2º Football Club"};
    const std::string COUNTRY{"EN"};
    const std::string YEAR{"2023"};

    net::football_client client{};
    client.connect(HOST, PORT);
    EXPECT_EQ(client.is_connected(), true);

    auto new_champ = net::new_championship{};
    std::copy(CHAMP.begin(), CHAMP.end(), new_champ.name.data());
    std::copy(COUNTRY.begin(), COUNTRY.end(), new_champ.country.data());
    net::message<net::event_type> msg;
    msg.header.id = net::event_type::new_championship;
    msg << new_champ;

    client.send(msg);

    auto new_club = net::new_club{};
    std::copy(CLUB.begin(), CLUB.end(), new_club.name.data());
    std::copy(COUNTRY.begin(), COUNTRY.end(), new_club.country.data());
    net::message<net::event_type> msg_club;
    msg_club.header.id = net::event_type::new_club;
    msg_club << new_club;

    client.send(msg_club);

    auto accepted{false};
    net::response champ_res{};
    net::response club_res{};
    net::response season_res{};
    net::response season_club_res{};

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::accept:
                    accepted = true;
                    break;
                case net::event_type::new_championship:
                    msg >> champ_res;
                    break;
                case net::event_type::new_club:
                    msg >> club_res;
                    break;
                }
            }

            if (accepted && champ_res.id && club_res.id)
            {
                break;
            }
        }
    }

    EXPECT_EQ(accepted, true);
    EXPECT_NE(champ_res.id, 0);
    EXPECT_EQ(champ_res.success, true);

    EXPECT_NE(club_res.id, 0);
    EXPECT_EQ(club_res.success, true);

    std::string cs_id = std::to_string(champ_res.id);
    football::championship cs{cs_id};
    EXPECT_EQ(cs.id(), cs_id);
    EXPECT_EQ(cs.name(), CHAMP);
    EXPECT_EQ(cs.country(), COUNTRY);

    std::string cl_id = std::to_string(club_res.id);
    football::club cl{cl_id};
    EXPECT_EQ(cl.id(), cl_id);
    EXPECT_EQ(cl.name(), CLUB);
    EXPECT_EQ(cl.country(), COUNTRY);

    auto new_season = net::new_season{};
    new_season.championship = cs.id_int();
    std::copy(YEAR.begin(), YEAR.end(), new_season.year.data());
    net::message<net::event_type> msg_season;
    msg_season.header.id = net::event_type::new_season;
    msg_season << new_season;

    client.send(msg_season);

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::new_season:
                    msg >> season_res;
                    break;
                }
            }

            if (season_res.id)
            {
                break;
            }
        }
    }

    EXPECT_NE(season_res.id, 0);
    EXPECT_EQ(season_res.success, true);

    std::string ss_id = std::to_string(season_res.id);
    football::season ss{ss_id, cs};
    EXPECT_EQ(ss.id(), ss_id);
    EXPECT_EQ(ss.year(), YEAR);
    EXPECT_EQ(ss.championshp().id(), cs.id());

    net::new_season_club sscl{};
    sscl.season = ss.id_int();
    sscl.club = cl.id_int();
    net::message<net::event_type> msg_season_club;
    msg_season_club.header.id = net::event_type::new_season_club;
    msg_season_club << sscl;

    client.send(msg_season_club);

    if (client.is_connected())
    {
        while (true)
        {
            if (!client.incoming().empty())
            {
                auto msg = client.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case net::event_type::new_season_club:
                    msg >> season_club_res;
                    break;
                }
            }

            if (season_club_res.id)
            {
                break;
            }
        }
    }

    EXPECT_NE(season_club_res.id, 0);
    EXPECT_EQ(season_club_res.success, true);

    std::string res_ss_id = std::to_string(season_club_res.id);
    football::season ss2{res_ss_id, cs};
    EXPECT_EQ(ss2.id(), ss_id);
    EXPECT_EQ(ss2.year(), YEAR);
    EXPECT_EQ(ss2.championshp().id(), cs.id());
    EXPECT_EQ(ss2.club_count(), 1);

    football::club cl2 = ss2.club_at(0);
    EXPECT_EQ(cl2.id(), cl.id());
    EXPECT_EQ(cl2.name(), cl.name());
    EXPECT_EQ(cl2.country(), cl.country());

    auto removed = ss.remove();
    EXPECT_EQ(removed, true);

    removed = cl.remove();
    EXPECT_EQ(removed, true);

    removed = cs.remove();
    EXPECT_EQ(removed, true);

    football::championship cs2{cs_id};
    EXPECT_EQ(cs2.found(), false);

    football::club cl3{cl_id};
    EXPECT_EQ(cl3.found(), false);

    football::season ss3{ss_id};
    EXPECT_EQ(ss3.found(), false);
}