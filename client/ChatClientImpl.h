//
// Created by Herman on 4/16/2022.
//
#include "ChatClient.h"

#include <cstdlib>
#include <utility>

using boost::asio::ip::tcp;

class ChatClient::ChatClientImpl
{
public:
    ChatClientImpl(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints, std::function<void (ChatPacket&&)> packetConsumer)
            : io_context_(io_context),
              socket_(io_context),
              packetConsumer(std::move(packetConsumer))
    {
        do_connect(endpoints);
    }

    void write(ChatPacket msg)
    {
        boost::asio::post(io_context_,
                          [this, msg=std::move(msg)]() mutable
                          {
                              bool write_in_progress = !write_msgs_.empty();
                              write_msgs_.emplace_back(std::move(msg));
                              if (!write_in_progress)
                              {
                                  do_write();
                              }
                          });
    }

    void close()
    {
        boost::asio::post(io_context_, [this]() { socket_.close(); });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints)
    {
        boost::asio::async_connect(socket_, endpoints,
                                   [this](boost::system::error_code ec, const tcp::endpoint&)
                                   {
                                       if (!ec)
                                       {
                                           do_read_header();
                                       }
                                   });
    }

    void do_read_header()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), ChatPacket::header_length),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }

    void do_read_body()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        packetConsumer(std::move(read_msg_));
                                        do_read_header();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }

    void do_write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         socket_.close();
                                     }
                                 });
    }

private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    ChatPacket read_msg_;
    ChatPacketQueue write_msgs_;
    std::function<void (ChatPacket&&)> packetConsumer;
};
