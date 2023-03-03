#include "pch.h"

TEST(Football_Server, start_server_thread)
{
    net::football_client client{};
    client.connect("127.0.0.1", 60000);
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

    // test::sleep(1);

    EXPECT_EQ(accepted, true);
    EXPECT_EQ(answered, true);
}

TEST(Football_Server, new_championship_event)
{
    const std::string CHAMP{"Prêmier Léague"};
    const std::string COUNTRY{"EN"};

    net::football_client client{};
    client.connect("127.0.0.1", 60000);
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
                    std::cout << "new_championship\n";
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

    // test::sleep(1);

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
    client.connect("127.0.0.1", 60000);
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
                    std::cout << "champ: " << champ_res.id << '\n';
                    break;
                case net::event_type::new_club:
                    msg >> club_res;
                    std::cout << "club: " << club_res.id << '\n';
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

    std::string str_id = std::to_string(champ_res.id);
    football::championship cs{str_id};
    EXPECT_EQ(cs.id(), str_id);
    EXPECT_EQ(cs.name(), CHAMP);
    EXPECT_EQ(cs.country(), COUNTRY);

    auto removed = cs.remove();
    EXPECT_EQ(removed, true);

    football::championship cs2{str_id};
    EXPECT_EQ(cs2.found(), false);
}