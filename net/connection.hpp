#pragma once

#include "common.hpp"
#include "message.hpp"
#include "tsqueue.hpp"

namespace net
{

template <typename T> class server_interface;

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

        if (m_nOwnerType == owner::server)
        {
            m_nHandshake_out = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
            m_nHandshake_check = scramble(m_nHandshake_out);
        }
        else
        {
            m_nHandshake_in = 0;
            m_nHandshake_out = 0;
        }
    }

    virtual ~connection() = default;

    [[nodiscard]] auto Id() const -> uint32_t
    {
        return id;
    }

    void connect_to_client(net::server_interface<T> *server, uint32_t uid = 0)
    {
        if (m_nOwnerType == owner::server)
        {
            if (m_socket.is_open())
            {
                id = uid;
                read_header();
                // write_validation();
                // read_validation(server);
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
                                               // read_validation();
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

    uint64_t m_nHandshake_out{0};
    uint64_t m_nHandshake_in{0};
    uint64_t m_nHandshake_check{0};

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
                                         std::cout << "[" << id << "] Write Header Fail: " << ec.message() << ".\n";
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
                                    else if (ec.value() != boost::asio::error::eof)
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

        read_header();
    }

    auto scramble(uint64_t input) -> uint64_t
    {
        uint64_t out = input ^ 0xC0DECAFEDEADBEEF;
        out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
        return out ^ 0x12345678C0DEFACE;
    }

    void write_validation()
    {
        boost::asio::async_write(m_socket, boost::asio::buffer(&m_nHandshake_out, sizeof(uint64_t)),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec)
                                     {
                                         if (m_nOwnerType == owner::client)
                                         {
                                             read_header();
                                         }
                                     }
                                     else
                                     {
                                         m_socket.close();
                                     }
                                 });
    }

    void read_validation(net::server_interface<T> *server = nullptr)
    {
        boost::asio::async_read(m_socket, boost::asio::buffer(&m_nHandshake_in, sizeof(uint64_t)),
                                [this, server](std::error_code ec, std::size_t length) {
                                    if (!ec)
                                    {
                                        if (m_nOwnerType == owner::server)
                                        {
                                            if (m_nHandshake_in == m_nHandshake_check)
                                            {
                                                std::cout << "Client validated\n";
                                                server->on_client_validated(this->shared_from_this());
                                                read_header();
                                            }
                                            else
                                            {
                                                std::cout << "Client disconnected (fail validation)\n";
                                                m_socket.close();
                                            }
                                        }
                                        else
                                        {
                                            m_nHandshake_out = scramble(m_nHandshake_in);
                                            write_validation();
                                        }
                                    }
                                    else
                                    {
                                        std::cout << "Client disconnected (read_validation)\n";
                                        m_socket.close();
                                    }
                                });
    }
};

} // namespace net