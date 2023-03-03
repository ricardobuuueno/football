#pragma once

#include "common.hpp"
#include "message.hpp"
#include "tsqueue.hpp"

namespace net
{

template <typename T> class client_interface
{
  public:
    client_interface()
    {
    }

    virtual ~client_interface()
    {
        disconnect();
    }

    bool connect(const std::string &host, const uint16_t port)
    {
        try
        {
            boost::asio::ip::tcp::resolver resolver(m_context);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context,
                                                           boost::asio::ip::tcp::socket(m_context), m_qMessagesIn);

            m_connection->connect_to_server(endpoints);

            thrContext = std::thread([this]() { m_context.run(); });
        }
        catch (std::exception &e)
        {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    void disconnect()
    {
        if (is_connected())
        {
            m_connection->disconnect();
        }

        m_context.stop();

        if (thrContext.joinable())
        {
            thrContext.join();
        }

        m_connection.release();

        std::cerr << "Client disconnected.\n";
    }

    bool is_connected()
    {
        if (m_connection)
        {
            return m_connection->is_connected();
        }
        else
        {
            return false;
        }
    }

    void send(const message<T> &msg)
    {
        if (is_connected())
        {
            m_connection->send(msg);
        }
    }

    tsqueue<owned_message<T>> &incoming()
    {
        return m_qMessagesIn;
    }

  protected:
    boost::asio::io_context m_context;
    std::thread thrContext;
    std::unique_ptr<connection<T>> m_connection;

  private:
    tsqueue<owned_message<T>> m_qMessagesIn;
};

} // namespace net