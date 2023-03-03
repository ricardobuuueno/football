#pragma once

#include "event.hpp"
#include "event_action.hpp"
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
            auto res = execute_action<net::new_championship>(msg);
            client->send(res);
        }
        break;
        case event_type::new_club: {
            auto res = execute_action<net::new_club>(msg);
            client->send(res);
        }
        break;
        }
    }

  private:
    template <typename T> auto execute_action(net::message<event_type> &msg) -> net::message<net::event_type>
    {
        T new_object;
        msg >> new_object;

        auto res = event_action(new_object);

        net::message<net::event_type> res_msg;
        res_msg.header.id = new_object.type;
        res_msg << res;

        return res_msg;
    }
};

} // namespace net