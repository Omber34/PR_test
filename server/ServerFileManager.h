//
// Created by Herman on 1/24/2022.
//

#ifndef PR_TEST_SERVERFILEMANAGER_H
#define PR_TEST_SERVERFILEMANAGER_H


#include <string>
#include <ChatFilePacket.h>

class ServerFileManager {

public:
    static ServerFileManager& getInstance();
    bool isDone(ChatPacket &packet);
    ChatPacket getDone(ChatPacket &packet);

private:
    ServerFileManager() = default;
    ServerFileManager(const ServerFileManager &other) = delete;
    ServerFileManager(ServerFileManager &&other) = delete;
    ServerFileManager& operator=(const ServerFileManager &other) = delete;
    ServerFileManager& operator=(ServerFileManager &&other) = delete;
    
    void startNewFile(ChatPacket &packet);
    bool continueFile(ChatPacket &packet);
    std::unordered_map<std::string, ChatFilePacket> unfinishedFiles;
};


#endif //PR_TEST_SERVERFILEMANAGER_H
