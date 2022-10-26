//
// Created by Herman on 1/17/2022.
//

#include <list>
#include <set>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include "ChatServer.h"
#include "ChatPacket.h"
#include "CoreUtility.h"
#include "Logger.h"

#include <boost/asio.hpp>

class chat_participant
{
public:
  virtual ~chat_participant()
  {}

  virtual void deliver(const core::ChatPacket &msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

using boost::asio::ip::tcp;
using core::Logger;

class chat_room
{
public:
  void join(chat_participant_ptr participant)
  {
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->deliver(msg);
  }

  void leave(chat_participant_ptr participant)
  {
    participants_.erase(participant);
  }

  void deliver(const core::ChatPacket &msg)
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto participant: participants_)
    {
      Logger::info("send");
      Logger::logPacket(msg);
      participant->deliver(msg);
    }
  }

private:
  std::set<chat_participant_ptr> participants_;
  enum
  {
    max_recent_msgs = 100
  };
  core::ChatPacketQueue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
  : public chat_participant
    , public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room &room)
    : socket_(std::move(socket)), room_(room)
  {
  }

  void start()
  {
    room_.join(shared_from_this());
    Logger::info("start session");
    do_read_header();
  }

  void deliver(const core::ChatPacket &msg)
  {
    Logger::info("deliver");
    Logger::logPacket(msg);
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.data(), core::ChatPacket::header_length),
      [this, self](boost::system::error_code ec, std::size_t length)
      {
        Logger::info("start reading");
        if (!ec && read_msg_.decode_header())
        {
          do_read_body();
        } else
        {
          Logger::critical(fmt::format("error wile reading header: '{}'", ec.message()));
          leave_room();
        }
      });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    boost::asio::async_read(socket_,
      boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          Logger::info("consumed");
          Logger::logPacket(read_msg_);

          room_.deliver(read_msg_);
          do_read_header();
        } else
        {
          Logger::critical(fmt::format("error wile reading body: '{}'", ec.message()));
          leave_room();
        }
      });
  }

  void do_write()
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
      boost::asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          Logger::info("server::write");
          Logger::logPacket(write_msgs_.front());
          write_msgs_.pop_front();
          if (!write_msgs_.empty())
          {
            do_write();
          }
        } else
        {
          Logger::critical(fmt::format("error wile writing: '{}'", ec.message()));
          leave_room();
        }
      });
  }

  void leave_room()
  {
    core::Logger::info("drop session");
    room_.leave(shared_from_this());
  }

  boost::asio::ip::tcp::socket socket_;
  chat_room &room_;
  core::ChatPacket read_msg_;
  core::ChatPacketQueue write_msgs_;
};

class chat_server
{
public:
  chat_server(boost::asio::io_context &io_context,
    const boost::asio::ip::tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<chat_session>(std::move(socket), room_)->start();
        }
        else
        {
          Logger::critical(fmt::format("error while accept: '{}'", ec.message()));
        }

        do_accept();
      });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

namespace server
{
  ChatServer::ChatServer()
  {
    boost::asio::io_context io_context;

    std::list<chat_server> servers;
    tcp::endpoint endpoint(tcp::v4(), std::stoi(core::DefaultPort));
    servers.emplace_back(io_context, endpoint);

    io_context.run();
  }

}