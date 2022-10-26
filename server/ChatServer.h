//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATSERVER_H
#define PR_TEST_CHATSERVER_H

#include <ChatAppModel.h>
#include <string>
#include <vector>
#include <memory>

namespace server
{
  class ChatServer
  {
  public:
    explicit ChatServer();

    ChatServer(const ChatServer &other) = delete;
    ChatServer(ChatServer &&other) = delete;
    ChatServer &operator=(const ChatServer &other) = delete;
    ChatServer &operator=(ChatServer &&other) = delete;

  private:
    core::ChatAppModelPtr model;
  };
}


#endif //PR_TEST_CHATSERVER_H
