//
// Created by Herman on 1/24/2022.
//

#ifndef PR_TEST_FILEMANAGER_H
#define PR_TEST_FILEMANAGER_H
#include <filesystem>
#include "ChatFilePacket.h"

class FileManager {
public:
    static std::string generateNewFileName(const ChatFilePacket& filePacket);
    static std::string generateNewFileName(const ChatEvent& chatEvent);
    static void saveFilePacketToDisk(const std::filesystem::path& destPath, const ChatFilePacket& packet);
    static void saveFilePacketToDisk(std::ostream & destFile, const ChatFilePacket& packet);
    static ChatFilePacket loadFileToFilePacket(const std::filesystem::path& sourcePath);
    static ChatFilePacket loadFileToFilePacket(std::istream& file);


    virtual ~FileManager() = 0 {};
private:
    static size_t nextEventId;
};


#endif //PR_TEST_FILEMANAGER_H
