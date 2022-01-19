//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATSERVER_H
#define PR_TEST_CHATSERVER_H


#include <string>
#include <vector>
#include <memory>

class ChatServer {
public:
    explicit ChatServer(const std::vector<std::string> &ports);

    ChatServer() = delete;
    ChatServer(const ChatServer &other) = delete;
    ChatServer(ChatServer &&other) = delete;
    ChatServer& operator=(const ChatServer &other) = delete;
    ChatServer& operator=(ChatServer &&other) = delete;
};


#endif //PR_TEST_CHATSERVER_H
