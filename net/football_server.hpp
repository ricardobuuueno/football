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
        case event_type::new_championship: {
            net::new_championship new_champ;
            msg >> new_champ;
            // std::cout << new_champ.name << '\n';

            // net::response res{"1"};
            // msg << res;
            // std::cout << "response\n";

            // net::message<net::event_type> res_msg;
            // res_msg.header.id = event_type::new_championship;
            // res_msg << res;

            client->send(msg);
        }
        break;
        }
    }
};

} // namespace net