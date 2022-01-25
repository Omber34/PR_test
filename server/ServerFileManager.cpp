//
// Created by Herman on 1/24/2022.
//

#include <CoreUtility.h>
#include "ServerFileManager.h"

bool ServerFileManager::isDone(ChatPacket &packet) {
    if (packet.event_id() == 0) {
        startNewFile(packet);
        return false;
    }
    return continueFile(packet);
}

ChatPacket ServerFileManager::getDone(ChatPacket &packet) {
    auto node = unfinishedFiles.extract(unfinishedFiles.begin());
    return *node.mapped().packets.begin();
}

void ServerFileManager::startNewFile(ChatPacket &packet) {
    auto headerEvent = CoreUtility::eventFromPacket(packet);
    ChatFilePacket filePacket;
    filePacket.expectedCount = headerEvent.packetCount;
    filePacket.packets.insert(packet);
    unfinishedFiles[headerEvent.user.toStdString() + headerEvent.message.message.toStdString()] = filePacket;
}

bool ServerFileManager::continueFile(ChatPacket &packet) {
    if (unfinishedFiles.empty())
        return false;
    if (unfinishedFiles.size() > 1)
        return false;
    auto filePacket = unfinishedFiles.begin()->second;
    filePacket.packets.insert(packet);
    return filePacket.packets.size() == filePacket.expectedCount;
}

ServerFileManager &ServerFileManager::getInstance() {
    static ServerFileManager instance;
    return instance;
}
