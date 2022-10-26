//
// Created by Herman on 4/16/2022.
//
#include "ChatClient.h"
#include "Logger.h"

#include <cstdlib>
#include <utility>

using boost::asio::ip::tcp;
using core::Logger;
class client::ChatClient::ChatClientImpl
{
public:
  ChatClientImpl(boost::asio::io_context &io_context, const tcp::resolver::results_type &endpoints, std::function<void(core::ChatPacket &&)> packetConsumer)
    : io_context_(io_context), socket_(io_context), packetConsumer(std::move(packetConsumer))
  {
    do_connect(endpoints);
  }

  void write(core::ChatPacket msg)
  {
    Logger::info("write");
    Logger::logPacket(msg);
    boost::asio::post(io_context_,
      [this, msg = std::move(msg)]() mutable
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
    boost::asio::post(io_context_, [this]()
    { close_connection(); });
  }

private:
  void do_connect(const tcp::resolver::results_type &endpoints)
  {
    boost::asio::async_connect(socket_, endpoints,
      [this](boost::system::error_code ec, const tcp::endpoint &)
      {
        if (!ec)
        {
          Logger::info("connect to server");
          do_read_header();
        }
        else
        {
          Logger::critical(fmt::format("couldn't connect to server: '{}'", ec.message()));
        }
      });
  }

  void do_read_header()
  {
    boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.data(), core::ChatPacket::header_length),
      [this](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec && read_msg_.decode_header())
        {
          Logger::info("read the header");
          do_read_body();
        } else
        {
          Logger::critical(fmt::format("fail read header: '{}'", ec.message()));
          close_connection();
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
          core::Logger::info("read");

          core::Logger::logEvent(core::CoreUtility::eventFromPacket(read_msg_));
          packetConsumer(std::move(read_msg_));
          do_read_header();
        } else
        {
          Logger::critical(fmt::format("fail read body: '{}'", ec.message()));
          close_connection();
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
            Logger::info("write packet");
            do_write();
          }
        } else
        {
          Logger::critical(fmt::format("fail write packet: '{}'", ec.message()));
          close_connection();
        }
      });
  }

private:

  void close_connection()
  {
    Logger::info("connection closed");
    socket_.close();
  }

  boost::asio::io_context &io_context_;
  tcp::socket socket_;
  core::ChatPacket read_msg_;
  core::ChatPacketQueue write_msgs_;
  std::function<void(core::ChatPacket &&)> packetConsumer;
};
