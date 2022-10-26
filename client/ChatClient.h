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

namespace client
{
  class ChatClient : public IChatClient
  {
  Q_OBJECT
  public slots:
    void SendEvent(core::ChatEvent event) Q_DECL_OVERRIDE;

  public:
    explicit ChatClient();

    void Disconnect();

    ChatClient(const ChatClient &other) = delete;
    ChatClient(ChatClient &&other) = delete;
    ChatClient &operator=(const ChatClient &other) = delete;
    ChatClient &operator=(ChatClient &&other) = delete;

    ~ChatClient();

  private:

    void consumePacket(core::ChatPacket packet);

    ClientFileManager fileManager;

    boost::asio::io_context io_context;
    std::thread ioContextThread;

    class ChatClientImpl;

    std::unique_ptr<ChatClientImpl> impl;
    void sendJoin(const core::ChatEvent &event);
  };
}


#endif //PR_TEST_CHATCLIENT_H
