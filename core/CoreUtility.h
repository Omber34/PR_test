//
// Created by Herman on 1/18/2022.
//

#pragma once

#include "ChatPacket.h"

#include "ChatEvent.h"
#include "ChatFilePacket.h"
#include "AppEvent.h"

#include <algorithm>
#include <iterator>

namespace core
{
  constexpr const auto DefaultHost = "127.0.0.1";
  constexpr const auto DefaultPort = "4026";

  class CoreUtility
  {
  public:
    static ChatPacket packetFromEvent(const ChatEvent &event);

    static ChatFilePacket filePacketFromEvent(ChatEvent event);

    static ChatEvent eventFromPacket(const ChatPacket &packet);

    static ChatEvent eventFromFilePacket(const ChatFilePacket &packet);

    static AppEventPtr appEventFromPacket(const ChatPacket &packet);

    static ChatPacket packetFromAppEvent(const AppEventPtr &packet);
  };
}
