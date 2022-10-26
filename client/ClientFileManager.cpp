//
// Created by Herman on 1/24/2022.
//

#include <CoreUtility.h>
#include <FileManager.h>
#include <QFileInfo>
#include "ClientFileManager.h"

namespace client
{
  std::optional<core::ChatPacket> ClientFileManager::process(core::ChatPacket packet)
  {
    auto packetId = packet.event_id();
    auto it = unfinishedFiles.find(packet.event_id());
    if (it == unfinishedFiles.end())
    {
      auto headerEvent = core::CoreUtility::eventFromPacket(packet);
      core::ChatFilePacket filePacket;
      filePacket.expectedCount = headerEvent.packetCount;
      unfinishedFiles[packet.event_id()] = filePacket;
      it = unfinishedFiles.insert(std::make_pair(packetId, std::move(filePacket))).first;
    }

    auto &filePacket = it->second;
    filePacket.packets.push_back(std::move(packet));

    if (filePacket.packets.size() != filePacket.expectedCount)
    {
      return {};
    }

    auto node = unfinishedFiles.extract(packetId);
    core::FileManager::saveFilePacketToDisk(getDownloadFilename(node.mapped().packets.front()), node.mapped());
    return *node.mapped().packets.begin();
  }

  std::string ClientFileManager::getDownloadFilename(const core::ChatEvent &event)
  {
    auto downloadPath = std::filesystem::path(event.user.toStdString());
    std::filesystem::create_directories(downloadPath);
    QFileInfo info(event.message.message);
    downloadPath.append(info.fileName().toStdString());

    return downloadPath.string();
  }

  std::string ClientFileManager::getDownloadFilename(const core::ChatPacket &packet)
  {
    return getDownloadFilename(core::CoreUtility::eventFromPacket(packet));
  }
}
