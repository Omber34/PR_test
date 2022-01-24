//
// Created by Herman on 1/24/2022.
//

#include "FileManager.h"
#include "CoreUtility.h"
#include <fstream>
#include <algorithm>

void FileManager::saveFilePacketToDisk(const std::filesystem::path &destPath, const ChatFilePacket& filePacket) {
    std::ofstream fileStream(destPath, std::ios::out | std::ios::binary);
    for (const auto& packet : filePacket.packets) {
        if (packet.sequence_index() == 0) // packet with sequence_index == 0 - its filename
            continue;
        fileStream.write(packet.body(), packet.body_length());
    }
}

ChatFilePacket FileManager::loadFileToFilePacket(const std::filesystem::path &sourcePath) {
    std::ifstream fileStream(sourcePath, std::ios::in | std::ios::binary);
    size_t fileSize = std::filesystem::file_size(sourcePath);
    ChatFilePacket result;
    size_t sequenceIndex = 1;// 0 is packet with event data

    std::string fileName = sourcePath.filename().string();

    while (fileStream.is_open() && !fileStream.eof()) {
        auto nextSize = std::min(static_cast<size_t>(ChatPacket::max_body_length), fileSize);
        ChatPacket packet;
        packet.body_length(nextSize);
        packet.sequence_index(sequenceIndex++);
        auto it = result.packets.insert(packet);

        fileStream.read(const_cast<char *>(it.first->body()), nextSize);
        if (nextSize < ChatPacket::max_body_length) {
            break;
        }
        fileSize -= nextSize;
    }
    fileStream.close();
    return result;
}

std::string FileManager::generateNewFileName(const ChatEvent &chatEvent) {
    return chatEvent.user.toStdString() + chatEvent.message.message.toStdString();
}

std::string FileManager::generateNewFileName(const ChatFilePacket &filePacket) {
    if (filePacket.packets.empty())
        return "";
    return generateNewFileName(CoreUtility::eventFromFilePacket(filePacket));


}