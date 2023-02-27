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

    test::sleep(1);

    EXPECT_EQ(accepted, true);
    EXPECT_EQ(answered, true);
}

TEST(Football_Server, new_championship_event)
{
    const std::string CHAMP{"Premier League"};
    const std::string COUNTRY{"EN"};

    net::football_client client{};
    client.connect("127.0.0.1", 60000);
    EXPECT_EQ(client.is_connected(), true);

    auto new_champ = net::new_championship{CHAMP, COUNTRY};
    net::message<net::event_type> msg;
    msg.header.id = net::event_type::new_championship;
    msg << new_champ;

    client.send(msg);

    auto accepted{false};
    net::response resp{};

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
                    // msg >> resp;
                    // std::cout << resp.id << '\n';
                    break;
                }
            }

            if (accepted /* && !resp.id.empty() */)
            {
                break;
            }
        }
    }

    EXPECT_EQ(accepted, true);
}