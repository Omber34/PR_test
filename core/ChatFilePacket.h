//
// Created by Herman on 1/23/2022.
//

#ifndef PR_TEST_CHATFILEPACKET_H
#define PR_TEST_CHATFILEPACKET_H


#include <vector>
#include "ChatPacket.h"

namespace core
{
  struct ChatFilePacket
  {
    std::vector<ChatPacket> packets;
    size_t expectedCount;
  };
}


#endif //PR_TEST_CHATFILEPACKET_H
