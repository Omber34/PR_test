//
// Created by Herman on 1/24/2022.
//

#ifndef PR_TEST_CLIENTFILEMANAGER_H
#define PR_TEST_CLIENTFILEMANAGER_H

#include <string>
#include <optional>
#include <ChatFilePacket.h>

class ClientFileManager {
public:
    static std::string getDownloadFilename(const ChatEvent &event);
    static std::string getDownloadFilename(const ChatPacket &packet);
    std::optional<ChatPacket>  process(ChatPacket packet);
private:

    std::unordered_map<size_t, ChatFilePacket> unfinishedFiles;
};


#endif //PR_TEST_CLIENTFILEMANAGER_H
