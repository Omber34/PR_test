//
// Created by Herman on 1/18/2022.
//

#ifndef PR_TEST_COREUTILITY_H
#define PR_TEST_COREUTILITY_H


#include "ChatPacket.h"
#include "ChatEvent.h"

constexpr const auto defaultHost = "127.0.0.1";
constexpr const auto defaultPort = "4026";

class CoreUtility {
public:
    static ChatPacket packetFromEvent(const ChatEvent& event);
    static ChatEvent eventFromPacket(ChatPacket &packet);

};


#endif //PR_TEST_COREUTILITY_H
