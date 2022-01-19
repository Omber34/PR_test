//
// Created by Herman on 1/18/2022.
//

#ifndef PR_TEST_PACKETEVENTTRANSFORM_H
#define PR_TEST_PACKETEVENTTRANSFORM_H


#include "ChatPacket.h"
#include "ChatEvent.h"

class PacketEventTransform {
public:
    static ChatPacket packetFromEvent(const ChatEvent& event);
    static ChatEvent eventFromPacket(ChatPacket &packet);

};


#endif //PR_TEST_PACKETEVENTTRANSFORM_H
