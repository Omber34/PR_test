//
// Created by Herman on 1/24/2022.
//

#ifndef PR_TEST_CLIENTFILEMANAGER_H
#define PR_TEST_CLIENTFILEMANAGER_H

#include <string>
#include <optional>
#include <ChatFilePacket.h>
namespace client
{
class ClientFileManager {
public:
  static std::string getDownloadFilename(const core::ChatEvent &event);
  static std::string getDownloadFilename(const core::ChatPacket &packet);
  std::optional<core::ChatPacket>  process(core::ChatPacket packet);
private:

  std::unordered_map<size_t, core::ChatFilePacket> unfinishedFiles;
};
}


#endif //PR_TEST_CLIENTFILEMANAGER_H
