//
// Created by Herman on 1/24/2022.
//

#include "FileManager.h"
#include "CoreUtility.h"
#include <fstream>
#include <algorithm>

namespace core
{
  size_t FileManager::nextEventId = 0;

  void FileManager::saveFilePacketToDisk(const std::filesystem::path &destPath, const ChatFilePacket &filePacket)
  {
    std::ofstream fileStream(destPath, std::ios::out | std::ios::binary);
    saveFilePacketToDisk(fileStream, filePacket);
  }

  ChatFilePacket FileManager::loadFileToFilePacket(const std::filesystem::path &sourcePath)
  {
    std::ifstream fileStream(sourcePath, std::ios::in | std::ios::binary);
    auto result = loadFileToFilePacket(fileStream);
    ChatEvent chatEvent;
    chatEvent.type = ChatEvent::EventType::PARTICIPANT_SHARE_FILE;
    auto filename = sourcePath.string();
    chatEvent.message = {QString::fromStdString(filename), true};
    result.packets.insert(result.packets.cbegin(), CoreUtility::packetFromEvent(chatEvent));
    return result;
  }

  std::string FileManager::generateNewFileName(const ChatEvent &chatEvent)
  {
    return chatEvent.user.toStdString() + chatEvent.message.message.toStdString();
  }

  std::string FileManager::generateNewFileName(const ChatFilePacket &filePacket)
  {
    if (filePacket.packets.empty())
      return "";
    return generateNewFileName(CoreUtility::eventFromFilePacket(filePacket));
  }

  ChatFilePacket FileManager::loadFileToFilePacket(std::istream &fileStream)
  {
    ChatFilePacket result;
    size_t sequenceIndex = 1;// 0 is packet with event data

    while (!fileStream.eof())
    {
      ChatPacket packet;
      packet.sequence_index(sequenceIndex++);
      packet.event_id(nextEventId);
      fileStream.read(reinterpret_cast<char *>(packet.body()), ChatPacket::max_body_length);
      auto nextSize = fileStream.gcount();
      packet.body_length(nextSize);

      packet.encode_header();
      result.packets.emplace_back(std::move(packet));

      if (nextSize < ChatPacket::max_body_length)
      {
        break;
      }
    }
    nextEventId++;
    result.expectedCount = sequenceIndex;
    return result;
  }

  void FileManager::saveFilePacketToDisk(std::ostream &fileStream, const ChatFilePacket &filePacket)
  {
    for (const auto &packet : filePacket.packets)
    {
      if (packet.sequence_index() == 0) // packet with event_id == 0 - its filename
        continue;
      fileStream.write(reinterpret_cast<const char *>(packet.body()), packet.body_length());
    }
  }
}
