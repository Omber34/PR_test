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
#include "PacketEventTransform.h"

#include <boost/asio.hpp>

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const ChatPacket& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

using boost::asio::ip::tcp;
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

    void deliver(const ChatPacket& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant: participants_)
            participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    ChatPacketQueue recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
        : public chat_participant
        , public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
            : socket_(std::move(socket)),
              room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const ChatPacket& msg)
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
                    auto event = PacketEventTransform::eventFromPacket(read_msg_);
                    std::cout << event.user.toStdString() << ": " << event.message.message.toStdString() << "\n";
                    room_.deliver(read_msg_);
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
    chat_room& room_;
    ChatPacket read_msg_;
    ChatPacketQueue write_msgs_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context,
    const boost::asio::ip::tcp::endpoint& endpoint)
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
                        std::cout << "we have a new member!!" << '\n';
                        std::make_shared<chat_session>(std::move(socket), room_)->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    chat_room room_;
};

ChatServer::ChatServer(const std::vector<std::string> &ports) {
    try
    {
        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        for (const auto & port : ports)
        {
            tcp::endpoint endpoint(tcp::v4(), std::stoi(port));
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    }
    catch (std::exception& e)
    {
        printf("Exception: %s\n", e.what());
    }
}