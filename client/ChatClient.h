//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATCLIENT_H
#define PR_TEST_CHATCLIENT_H

#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include "../core/ChatPacket.h"
#include "ChatModel.h"
#include "../core/PacketEventTransform.h"

class ChatClient {
public:
    ChatClient(const std::string& host, const std::string& port);

    void setUsername(const std::string &newUser);
    std::string getUsername() const;

    void SendPacket(ChatPacket packet);

    void Disconnect();

    ChatClient() = delete;
    ChatClient(const ChatClient &other) = delete;
    ChatClient(ChatClient &&other) = delete;
    ChatClient& operator=(const ChatClient &other) = delete;
    ChatClient& operator=(ChatClient &&other) = delete;

    ~ChatClient();

private:
    boost::asio::io_context io_context;
    std::thread ioContextThread;
    class ChatClientImpl;
    std::unique_ptr<ChatClientImpl> impl;
    ChatModel model;
};


#endif //PR_TEST_CHATCLIENT_H
