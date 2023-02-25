#include "pch.h"

const uint16_t PORT{60000};

/* auto launch_server(uint16_t port) -> std::future<void> &&
{
    std::future<void> server_future = std::async(std::launch::async, [&port]() {
        net::football_server server(port);
        server.start();
        while (true)
        {
            server.update();
        }
    });

    return std::move(server_future);
} */

TEST(Client_Server, start_server_thread)
{

    std::future<void> server_future = std::async(std::launch::async, [&]() {
        net::football_server server(PORT);
        server.start();
        while (true)
        {
            server.update();
        }
    });

    net::football_client client{};
    client.connect("127.0.0.1", PORT);

    client.ping_server();

    server_future.get();
}