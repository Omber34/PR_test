//
// Created by Herman on 1/24/2022.
//

#include <CoreUtility.h>
#include <FileManager.h>
#include <iostream>
#include <QFileInfo>
#include "ClientFileManager.h"

bool ClientFileManager::isDone(ChatPacket &packet) {
    if (unfinishedFiles.find(packet.event_id()) != unfinishedFiles.end()) {
        return continueFile(packet);
    }
    if (packet.sequence_index() == 0) {
        startNewFile(packet);
        return false;
    }
    return false;
}

ChatPacket ClientFileManager::getDone(ChatPacket &packet) {
    auto node = unfinishedFiles.extract(packet.event_id());

    FileManager::saveFilePacketToDisk(getDownloadFilename(node.mapped().packets.front()), node.mapped());
    return *node.mapped().packets.begin();
}

void ClientFileManager::startNewFile(ChatPacket &packet) {
    auto headerEvent = CoreUtility::eventFromPacket(packet);
    ChatFilePacket filePacket;
    filePacket.expectedCount = headerEvent.packetCount;
    filePacket.packets.push_back(packet);
    unfinishedFiles[packet.event_id()] = filePacket;
}

bool ClientFileManager::continueFile(ChatPacket &packet) {
    if (unfinishedFiles.empty())
        return false;
    auto &filePacket = unfinishedFiles.find(packet.event_id())->second;
    filePacket.packets.push_back(packet);
    return filePacket.packets.size() == filePacket.expectedCount;
}

ClientFileManager &ClientFileManager::getInstance() {
    static ClientFileManager instance;
    return instance;
}

std::string ClientFileManager::getDownloadFilename(const ChatEvent &event) {
    auto downloadPath = std::filesystem::path(event.user.toStdString());
    std::filesystem::create_directories(downloadPath);
    QFileInfo info(event.message.message);
    downloadPath.append(info.fileName().toStdString());

    return downloadPath.string();
}

std::string ClientFileManager::getDownloadFilename(const ChatPacket &packet) {
    return getDownloadFilename(CoreUtility::eventFromPacket(packet));
}
