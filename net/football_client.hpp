#pragma once

#include "client.hpp"
#include "event.hpp"
#include "message.hpp"

namespace net
{

class football_client : public client_interface<event_type>
{
  public:
    void ping_server()
    {
        message<event_type> msg;
        msg.header.id = event_type::ping;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        send(msg);
    }
};

} // namespace net