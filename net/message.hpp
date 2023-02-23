#pragma once

#include "common.hpp"

namespace net
{

template <typename T> struct message_header
{
    T id{};
    uint32__t size{0};
};

template <typename T> struct message
{
    message_header<T> header{};
    std::vector<uint8_t> body;

    size_t size() const
    {
        return body.size();
    }

    friend std::ostream &operator<<(std::ostream &os, const message<T> &msg)
    {
        os << "[Id: " << int(msg.header.id) << ", Size: " << msg.header.size << "]";
        return os;
    }

    template <typename Data> friend message<T> &operator<<(message<T> &msg, const Data &data)
    {
        static_assert(std::is_standard_layout<Data>::value, "Data is too complex to be pushed into vector");
        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(Data));
        std::memcpy(msg.body.data() + i, &data, sizeof(Data));
        msg.header.size = msg.size();
        return msg;
    }

    template <typename Data> friend message<T> &operator>>(message<T> &msg, Data &data)
    {
        static_assert(std::is_standard_layout<Data>::value, "Data is too complex to be pulled from vector");
        size_t i = msg.body.size() - sizeof(Data);
        std::memcpy(&data, msg.body.data() + i, sizeof(Data));
        msg.body.resize(i);
        msg.header.size = msg.size();
        return msg;
    }

    template <typename T> class connection;

    template <typename T> struct owned_message
    {
        std::shared_ptr<connection<T>> remote = nullptr;
        message<T> msg;

        // Again, a friendly string maker
        friend std::ostream &operator<<(std::ostream &os, const owned_message<T> &owned)
        {
            os << owned.msg;
            return os;
        }
    };
};

} // namespace net