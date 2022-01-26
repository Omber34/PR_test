//
// Created by Herman on 1/24/2022.
//

#ifndef PR_TEST_CLIENTFILEMANAGER_H
#define PR_TEST_CLIENTFILEMANAGER_H

#include <string>
#include <ChatFilePacket.h>

class ClientFileManager {
public:
    static ClientFileManager& getInstance();
    bool isDone(ChatPacket &packet);
    ChatPacket getDone(ChatPacket &packet);
    std::string getDownloadFilename(const ChatEvent &event);
    std::string getDownloadFilename(const ChatPacket &packet);

private:
    ClientFileManager() = default;
    ClientFileManager(const ClientFileManager &other) = delete;
    ClientFileManager(ClientFileManager &&other) = delete;
    ClientFileManager& operator=(const ClientFileManager &other) = delete;
    ClientFileManager& operator=(ClientFileManager &&other) = delete;

    void startNewFile(ChatPacket &packet);
    bool continueFile(ChatPacket &packet);
    std::unordered_map<size_t, ChatFilePacket> unfinishedFiles;
};


#endif //PR_TEST_CLIENTFILEMANAGER_H
