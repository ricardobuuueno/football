#pragma once

#include "common.hpp"
#include "connection.hpp"
#include "message.hpp"
#include "tsqueue.hpp"

namespace net
{

template <typename T> class server_interface
{
  public:
    server_interface(uint16_t port)
        : m_asioAcceptor(m_asioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
    }

    virtual ~server_interface()
    {
        std::cout << "[SERVER] ~server_interface()\n";
        stop();
    }

    auto start() -> bool
    {
        try
        {
            wait_for_client_connection();
            m_threadContext = std::thread([this]() { m_asioContext.run(); });
        }
        catch (std::exception &e)
        {
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }

    void stop()
    {
        m_asioContext.stop();
        if (m_threadContext.joinable())
            m_threadContext.join();
        std::cout << "[SERVER] Stopped!\n";
    }

    void wait_for_client_connection()
    {
        m_asioAcceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec)
            {
                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                std::shared_ptr<connection<T>> newconn = std::make_shared<connection<T>>(
                    connection<T>::owner::server, m_asioContext, std::move(socket), m_qMessagesIn);

                if (on_client_connect(newconn))
                {
                    m_deqConnections.push_back(std::move(newconn));
                    m_deqConnections.back()->connect_to_client(this, nIDCounter++);
                    std::cout << "[" << m_deqConnections.back()->Id() << "] Connection Approved\n";
                }
                else
                {
                    std::cout << "[-----] Connection Denied\n";
                }
            }
            else
            {
                std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
            }

            wait_for_client_connection();
        });
    }

    void message_client(std::shared_ptr<connection<T>> client, const message<T> &msg)
    {
        if (client && client->is_connected())
        {
            client->send(msg);
        }
        else
        {
            on_client_disconnect(client);
            client.reset();
            m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client),
                                   m_deqConnections.end());
        }
    }

    void message_all_clients(const message<T> &msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
    {
        bool bInvalidClientExists = false;

        for (auto &client : m_deqConnections)
        {
            if (client && client->is_connected())
            {
                if (client != pIgnoreClient)
                    client->send(msg);
            }
            else
            {
                on_client_disconnect(client);
                client.reset();

                bInvalidClientExists = true;
            }
        }

        if (bInvalidClientExists)
        {
            m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr),
                                   m_deqConnections.end());
        }
    }

    auto update(size_t nMaxMessages = -1, bool bWait = false) -> void
    {
        if (bWait)
        {
            m_qMessagesIn.wait();
        }

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
        {
            auto msg = m_qMessagesIn.pop_front();

            on_message(msg.remote, msg.msg);

            nMessageCount++;
        }
    }

    auto no_inbound_message() -> bool
    {
        return m_qMessagesIn.empty();
    }

    virtual void on_client_validated(std::shared_ptr<connection<T>> client)
    {
    }

  protected:
    tsqueue<owned_message<T>> m_qMessagesIn;
    std::deque<std::shared_ptr<connection<T>>> m_deqConnections;
    boost::asio::io_context m_asioContext;
    std::thread m_threadContext;
    boost::asio::ip::tcp::acceptor m_asioAcceptor;
    uint32_t nIDCounter{10000};

    virtual auto on_client_connect(std::shared_ptr<connection<T>> client) -> bool
    {
        return false;
    }

    virtual void on_client_disconnect(std::shared_ptr<connection<T>> client)
    {
    }

    virtual void on_message(std::shared_ptr<connection<T>> client, message<T> &msg)
    {
    }
};

} // namespace net