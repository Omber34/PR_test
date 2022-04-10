//
// Created by Herman on 4/9/2022.
//

#include "gtest/gtest.h"
#include <algorithm>
#include "CoreUtility.h"

namespace {

    const char * getTestPacketData() {
        const auto packetBodyData = R"({ user:"Taras", message:"vstavaite, kaidany porvite i vrazhou zlou kroviu zemly okropite"})";
        char *packetData = new char[102];
        int32_t * cvtPacketData = reinterpret_cast<int32_t*>(packetData);
        cvtPacketData[0] = 1U;
        cvtPacketData[1] = 1U;
        cvtPacketData[2] = 90U;
        memcpy(packetData + ChatPacket::header_length, packetBodyData, strlen(packetBodyData));
        return packetData;
    }

    TEST(ChatPacket, TrivialMethod) {
        auto packetData = getTestPacketData() + 12;
        auto packetDataLen = strlen(packetData);
        ChatPacket packet;
        packet.event_id(1);
        packet.sequence_index(1);
        packet.body_length(packetDataLen);
        packet.encode_header();
        memcpy(packet.body(), packetData, strlen(packetData));
        packet.decode_header();

        EXPECT_EQ(packet.event_id(), 1U);
        EXPECT_EQ(packet.sequence_index(), 1U);
        EXPECT_EQ(packet.body_length(), packetDataLen);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, strlen(packetData)) == 0);
    }

    TEST(ChatEvent, Trivial) {

    }

    TEST(FileManager, Trivial) {

    }
}