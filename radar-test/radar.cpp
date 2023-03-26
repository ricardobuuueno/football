#include "pch.h"

const std::string HOST{"127.0.0.1"};
const uint16_t PORT{60000};

class RadarEnvironment : public ::testing::Environment
{
  public:
    RadarEnvironment(const std::string &conf_file) : config_file(conf_file)
    {
    }

    ~RadarEnvironment() override
    {
    }

    // Override this to define how to set up the environment.
    void SetUp() override
    {
        football::init(config_file);

        server_future = std::async(std::launch::async, [&]() {
            net::football_server server(PORT);
            server.start();
            while (true)
            {
                server.update();
                if (cancel_thread && server.no_inbound_message())
                {
                    break;
                }
            }
        });
    }

    // Override this to define how to tear down the environment.
    void TearDown() override
    {
        cancel_thread = true;
        std::cout << "radar::cancel_thread = true\n";
        server_future.get();
        std::cout << "radar::server_future.get()\n";
    }

  private:
    const std::string config_file;
    std::atomic_bool cancel_thread{ATOMIC_VAR_INIT(false)};
    std::future<void> server_future;
};

int main(int argc, char **argv)
{
    std::string config_filename{argv[0]};
    config_filename += ".toml";

    RadarEnvironment *env = new RadarEnvironment(config_filename);
    ::testing::AddGlobalTestEnvironment(env);

    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    return result;
}

TEST(radar, task_scheduler_test)
{
    using net::tsqueue;
    using scanner::new_championship_task;
    using scanner::ptask_result;
    using scanner::result_new_championship;
    using scanner::scheduler;
    using util::New;
    using util::NewShared;

    auto new_cs_task = New<new_championship_task>(pub::publisher::gazeta_esportiva,
                                                  "https://www.gazetaesportiva.com/campeonatos/paulista/", "BR", "16");
    bool saved = new_cs_task->save();
    EXPECT_EQ(saved, true);

    auto queue = NewShared<tsqueue<ptask_result>>();

    auto sched = New<scheduler>(queue);

    net::football_client client{queue};
    client.connect(HOST, PORT);

    client.listen();

    /* sched->wait();

            auto result = queue->pop_front();
            auto new_cs_result = dynamic_cast<result_new_championship *>(&(*result)); */
}

TEST(radar, find_new_championship)
{
    /*     std::string COUNTRY{"EN"};
        scanner::search_terms st{"Premier League"};

        auto [found, data, error_message] = scanner::search<net::new_championship>(st, COUNTRY);

        EXPECT_EQ(found, true);
        EXPECT_EQ(error_message, "");

        EXPECT_NE(data.name, "");
        EXPECT_NE(data.country, ""); */
}

TEST(radar, find_championship_clubs)
{
    //
}

TEST(radar, find_new_championship_season)
{
    //
}

TEST(radar, find_championship_games)
{
    //
}
