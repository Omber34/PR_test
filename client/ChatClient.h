//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATCLIENT_H
#define PR_TEST_CHATCLIENT_H

#include <memory>
#include <thread>

#include <boost/asio.hpp>

#include "ChatPacket.h"
#include "ChatModel.h"
#include "CoreUtility.h"
#include "IChatClient.h"
#include "ClientFileManager.h"

class ChatClient : public IChatClient {
    Q_OBJECT
public slots:
    void SendEvent(ChatEvent event) Q_DECL_OVERRIDE;
    
public:
    void Disconnect();

    static ChatClient& getInstance();
    ChatClient(const ChatClient &other) = delete;
    ChatClient(ChatClient &&other) = delete;
    ChatClient& operator=(const ChatClient &other) = delete;
    ChatClient& operator=(ChatClient &&other) = delete;

    ~ChatClient();

private:
    explicit ChatClient();

    void consumePacket(ChatPacket packet);

    ClientFileManager fileManager;

    boost::asio::io_context io_context;
    std::thread ioContextThread;
    class ChatClientImpl;
    std::unique_ptr<ChatClientImpl> impl;
};


#endif //PR_TEST_CHATCLIENT_H
