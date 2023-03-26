#pragma once

#include "client.hpp"
#include "event.hpp"
#include "message.hpp"

#include "../scanner/task_result.hpp"
#include "../util/util.hpp"

namespace net
{

class football_client : public client_interface<event_type>
{
  public:
    football_client() = default;

    explicit football_client(net::stsqueue<scanner::ptask_result> queue) : _queue(queue)
    {
    }

    auto ping_server() -> void
    {
        message<event_type> msg;
        msg.header.id = event_type::ping;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        send(msg);
    }

    auto listen() -> void
    {
        if (!is_connected())
        {
            return;
        }

        while (true)
        {
            while (!_queue->empty())
            {
                std::cout << "!empty\n";
                auto t_result = _queue->pop_front();
                switch (t_result->event_type())
                {
                case net::event_type::new_championship: {
                    auto new_champ_task =
                        util::New<scanner::task_result, scanner::result_new_championship>(t_result.get());
                    auto new_champ = new_champ_task->get();
                    net::message<net::event_type> msg;
                    msg.header.id = new_champ.type;
                    msg << new_champ;

                    std::cout << "!send\n";
                    send(msg);
                }
                break;
                }
            }

            while (true)
            {

                if (!incoming().empty())
                {
                    auto msg = incoming().pop_front().msg;
                    switch (msg.header.id)
                    {
                    case net::event_type::accept:
                        std::cout << "Accepted\n";
                        break;
                    case net::event_type::new_championship:
                        net::response res{};
                        msg >> res;
                        std::cout << res.id << '\n';
                        break;
                    }
                }
                else
                {
                    break;
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

  private:
    net::stsqueue<scanner::ptask_result> _queue;
};

} // namespace net