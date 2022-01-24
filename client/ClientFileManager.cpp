//
// Created by Herman on 1/24/2022.
//

#include <CoreUtility.h>
#include <FileManager.h>
#include "ClientFileManager.h"

bool ClientFileManager::isDone(ChatPacket &packet) {
    if (packet.sequence_index() == 0) {
        startNewFile(packet);
        return false;
    }
    return continueFile(packet);
}

ChatPacket ClientFileManager::getDone(ChatPacket &packet) {
    auto node = unfinishedFiles.extract(unfinishedFiles.begin());

    FileManager::saveFilePacketToDisk(node.key(), node.mapped());
    return *node.mapped().packets.begin();
}

void ClientFileManager::startNewFile(ChatPacket &packet) {
    auto headerEvent = CoreUtility::eventFromPacket(packet);
    ChatFilePacket filePacket;
    filePacket.expectedCount = headerEvent.packetCount;
    filePacket.packets.insert(packet);
    unfinishedFiles[headerEvent.user.toStdString() + headerEvent.message.message.toStdString()] = filePacket;
}

bool ClientFileManager::continueFile(ChatPacket &packet) {
    if (unfinishedFiles.empty())
        return false;
    if (unfinishedFiles.size() > 1)
        return false;
    auto filePacket = unfinishedFiles.begin()->second;
    filePacket.packets.insert(packet);
    return filePacket.packets.size() == filePacket.expectedCount;
}

ClientFileManager &ClientFileManager::getInstance() {
    static ClientFileManager instance;
    return instance;
}
