#pragma once

#include "event.hpp"
#include "server.hpp"

namespace net
{

class football_server : public server_interface<event_type>
{
  public:
    football_server(uint16_t nPort) : server_interface<event_type>(nPort)
    {
    }

  protected:
    auto on_client_connect(std::shared_ptr<connection<event_type>> client) -> bool override
    {
        message<event_type> msg;
        msg.header.id = event_type::accept;
        client->send(msg);
        return true;
    }

    void on_client_disconnect(std::shared_ptr<connection<event_type>> client) override
    {
        std::cout << "Removing client [" << client->Id() << "]\n";
    }

    void on_message(std::shared_ptr<connection<event_type>> client, message<event_type> &msg) override
    {
        switch (msg.header.id)
        {
        case event_type::ping: {
            std::cout << "[" << client->Id() << "]: Server Ping\n";
            client->send(msg);
        }
        break;
        }
    }
};

} // namespace net