#pragma once

#include "common.hpp"
#include "message.hpp"
#include "tsqueue.hpp"

namespace net
{

template <typename T> class connection : public std::enable_shared_from_this<connection<T>>
{
  public:
    enum class owner
    {
        server,
        client
    };

    connection(owner parent, boost::asio::io_context &asioContext, boost::asio::ip::tcp::socket socket,
               tsqueue<owned_message<T>> &qIn)
        : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
    {
        m_nOwnerType = parent;
    }

    virtual ~connection() = default;

    [[nodiscard]] auto Id() const -> uint32_t
    {
        return id;
    }

    void connect_to_client(uint32_t uid = 0)
    {
        if (m_nOwnerType == owner::server)
        {
            if (m_socket.is_open())
            {
                id = uid;
                read_header();
            }
        }
    }

    void connect_to_server(const boost::asio::ip::tcp::resolver::results_type &endpoints)
    {
        if (m_nOwnerType == owner::client)
        {
            boost::asio::async_connect(m_socket, endpoints,
                                       [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
                                           if (!ec)
                                           {
                                               read_header();
                                           }
                                       });
        }
    }

    void disconnect()
    {
        if (is_connected())
            boost::asio::post(m_asioContext, [this]() { m_socket.close(); });
    }

    [[nodiscard]] auto is_connected() const -> bool
    {
        return m_socket.is_open();
    }

    void start_listening()
    {
    }

    void send(const message<T> &msg)
    {
        boost::asio::post(m_asioContext, [this, msg]() {
            bool bWritingMessage = !m_qMessagesOut.empty();
            m_qMessagesOut.push_back(msg);
            if (!bWritingMessage)
            {
                write_header();
            }
        });
    }

  protected:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_context &m_asioContext;
    tsqueue<message<T>> m_qMessagesOut;
    tsqueue<owned_message<T>> &m_qMessagesIn;
    message<T> m_msgTemporaryIn;
    owner m_nOwnerType = owner::server;
    uint32_t id{0};

  private:
    void write_header()
    {
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec)
                                     {
                                         if (m_qMessagesOut.front().body.size() > 0)
                                         {
                                             write_body();
                                         }
                                         else
                                         {
                                             m_qMessagesOut.pop_front();

                                             if (!m_qMessagesOut.empty())
                                             {
                                                 write_header();
                                             }
                                         }
                                     }
                                     else
                                     {
                                         std::cout << "[" << id << "] Write Header Fail.\n";
                                         m_socket.close();
                                     }
                                 });
    }

    void write_body()
    {
        boost::asio::async_write(
            m_socket, boost::asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
            [this](std::error_code ec, std::size_t length) {
                if (!ec)
                {
                    m_qMessagesOut.pop_front();
                    if (!m_qMessagesOut.empty())
                    {
                        write_header();
                    }
                }
                else
                {
                    std::cout << "[" << id << "] Write Body Fail.\n";
                    m_socket.close();
                }
            });
    }

    void read_header()
    {
        boost::asio::async_read(m_socket, boost::asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
                                [this](std::error_code ec, std::size_t length) {
                                    if (!ec)
                                    {
                                        if (m_msgTemporaryIn.header.size > 0)
                                        {
                                            m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                                            read_body();
                                        }
                                        else
                                        {
                                            add_to_incoming_message_queue();
                                        }
                                    }
                                    else
                                    {
                                        std::cout << "[" << id << "] Read Header Fail: " << ec.message() << '\n';
                                        m_socket.close();
                                    }
                                });
    }

    void read_body()
    {
        boost::asio::async_read(m_socket,
                                boost::asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
                                [this](std::error_code ec, std::size_t length) {
                                    if (!ec)
                                    {
                                        add_to_incoming_message_queue();
                                    }
                                    else
                                    {
                                        std::cout << "[" << id << "] Read Body Fail.\n";
                                        m_socket.close();
                                    }
                                });
    }

    void add_to_incoming_message_queue()
    {
        if (m_nOwnerType == owner::server)
            m_qMessagesIn.push_back({this->shared_from_this(), m_msgTemporaryIn});
        else
            m_qMessagesIn.push_back({nullptr, m_msgTemporaryIn});

        // We must now prime the asio context to receive the next message. It
        // wil just sit and wait for bytes to arrive, and the message construction
        // process repeats itself. Clever huh?
        read_header();
    }
};

} // namespace net