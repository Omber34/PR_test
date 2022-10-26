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

#include <boost/asio.hpp>

using namespace core;

const QString publicRoomName = "Public room";

class ChatParticipant
{
public:
  virtual ~ChatParticipant() { }

  virtual QString getName() const = 0;
  virtual void setName(const QString &newName) = 0;

  virtual void deliver(const ChatPacket &msg) = 0;
};

typedef std::shared_ptr<ChatParticipant> ChatParticipantPtr;

//----------------------------------------------------------------------

using boost::asio::ip::tcp;

class IChatRoom
{
public:
  virtual void join(ChatParticipantPtr participant) = 0;
  virtual void leave(ChatParticipantPtr participant) = 0;
  virtual void deliver(ChatParticipantPtr participant, const ChatPacket &msg) = 0;
};

class Room : public IChatRoom
{
public:
  void join(ChatParticipantPtr participant) override
  {
    participants_.insert(participant);
    for (auto msg: recent_msgs_)
      participant->deliver(msg);
  }

  void leave(ChatParticipantPtr participant) override
  {
    participants_.erase(participant);
  }

  void deliver(ChatParticipantPtr participant, const ChatPacket &msg) override
  {
    recent_msgs_.push_back(msg);
    while (recent_msgs_.size() > max_recent_msgs)
      recent_msgs_.pop_front();

    for (auto participant: participants_)
      participant->deliver(msg);
  }

private:
  std::set<ChatParticipantPtr> participants_;
  enum
  {
    max_recent_msgs = 100
  };
  ChatPacketQueue recent_msgs_;
};

//----------------------------------------------------------------------

class WaitingChatRoom : public IChatRoom
{
public:
  WaitingChatRoom(std::shared_ptr<ChatAppModel> appModel, std::vector<::Room> &rooms)
          : rooms(rooms)
            , appModel(appModel) { }

  void join(ChatParticipantPtr participant) override
  {
    unauthorizedParticipants.insert(participant);
  }

  void leave(ChatParticipantPtr participant) override
  {
    if (participants.find(participant) != participants.end())
      participants.erase(participant);

    if (unauthorizedParticipants.find(participant) != unauthorizedParticipants.end())
      unauthorizedParticipants.erase(participant);
  }

  void deliver(ChatParticipantPtr participant, const ChatPacket &msg) override
  {
    if (unauthorizedParticipants.find(participant) != unauthorizedParticipants.end())
    {
      deliverUnauthorized(participant, msg);
    }

    if (participants.find(participant) != participants.end())
    {
      deliverAuthorized(participant, msg);
    }
  }

private:
  bool authorizeParticipant(ChatParticipantPtr participant, const ChatPacket &event)
  {
    const auto appEvent = CoreUtility::appEventFromPacket(event);

    if (appEvent->getType() != AppEventType::USER_JOIN)
      return false;

    participant->setName(appEvent->getUserName());
    return true;
  }

  void deliverUnauthorized(ChatParticipantPtr participant, const ChatPacket &msg)
  {
    if (authorizeParticipant(participant, msg))
    {
      std::lock_guard lockGuard {participant_mutex};
      auto node = unauthorizedParticipants.extract(participant);
      participants.insert(std::move(node));
    }
  }

  void deliverAuthorized(ChatParticipantPtr participant, const ChatPacket &msg)
  {
    for (auto aparticipant : participants)
    {
      aparticipant->deliver(msg);
    }
  }

  std::set<ChatParticipantPtr> participants;
  std::set<ChatParticipantPtr> unauthorizedParticipants;
  std::mutex participant_mutex;
  std::shared_ptr<ChatAppModel> appModel;
  std::vector<::Room> &rooms;
};

//----------------------------------------------------------------------

class ChatSession
        : public ChatParticipant
          , public std::enable_shared_from_this<ChatSession>
{
public:
  ChatSession(tcp::socket socket, IChatRoom &room)
          : socket_(std::move(socket))
            , room_(room)
  {
  }

  void start()
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  QString getName() const override
  {
    return username;
  }

  void setName(const QString &newName) override
  {
    username = newName;
  }

  void deliver(const ChatPacket &msg)
  {
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
                            boost::asio::buffer(read_msg_.data(), ChatPacket::header_length),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/)
                            {
                              if (!ec && read_msg_.decode_header())
                              {
                                do_read_body();
                              }
                              else
                              {
                                room_.leave(shared_from_this());
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
                                auto event = CoreUtility::eventFromPacket(read_msg_);
                                std::cout << event.user.toStdString() << " : " << event.message.message.toStdString()
                                          << "\n";

                                room_.deliver(shared_from_this(), read_msg_);
                                do_read_header();
                              }
                              else
                              {
                                room_.leave(shared_from_this());
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
                                 write_msgs_.pop_front();
                                 if (!write_msgs_.empty())
                                 {
                                   do_write();
                                 }
                               }
                               else
                               {
                                 room_.leave(shared_from_this());
                               }
                             });
  }

  boost::asio::ip::tcp::socket socket_;
  QString username;
  IChatRoom &room_;
  ChatPacket read_msg_;
  ChatPacketQueue write_msgs_;
};

//----------------------------------------------------------------------

class ChatServerImpl
{
public:
  ChatServerImpl(boost::asio::io_context &io_context,
                 const boost::asio::ip::tcp::endpoint &endpoint,
                 ChatAppModelPtr chatAppModel)
          : acceptor_(io_context, endpoint)
            , waitingChatRoom(chatAppModel, rooms)
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
                std::make_shared<ChatSession>(std::move(socket), waitingChatRoom)->start();
              }

              do_accept();
            });
  }

  tcp::acceptor acceptor_;
  std::vector<::Room> rooms;
  WaitingChatRoom waitingChatRoom;
};

namespace server
{
  ChatServer::ChatServer()
  {
    boost::asio::io_context io_context;

    std::list<ChatServerImpl> servers;
    tcp::endpoint endpoint(tcp::v4(), std::stoi(DefaultPort));
    servers.emplace_back(io_context, endpoint, model);

    io_context.run();

    CreateRoom newRoom{ publicRoomName, "" };
    model->getRoomModel()->CreateRoom(newRoom);
  }
}