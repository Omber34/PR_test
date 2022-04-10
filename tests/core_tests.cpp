//
// Created by Herman on 4/9/2022.
//

#include "gtest/gtest.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "CoreUtility.h"

namespace {
    const char * getTestPacketData() {
        const auto packetBodyData = R"({"message":"vstavaite, kaidany porvite i vrazhou zlou kroviu zemly okropite","type":0,"user":"Taras"})";
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

    TEST(ChatPacket, MessageFromEvent) {
        auto packetData = getTestPacketData() + 12;
        auto packetDataLen = strlen(packetData);
        auto json = QJsonDocument::fromJson(packetData).object();
        ChatEvent event;
        event.user = json["user"].toString();
        event.message.message = json["message"].toString();
        event.type = ChatEvent::PARTICIPANT_MESSAGE;

        auto packet = CoreUtility::packetFromEvent(event);

        EXPECT_EQ(packet.event_id(), 0);
        EXPECT_EQ(packet.sequence_index(), 0);
        EXPECT_EQ(packet.body_length(), packetDataLen);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, strlen(packetData)) == 0);
    }

    TEST(ChatEvent, FromPacket) {
        auto packetData = getTestPacketData() + 12;
        auto packetDataLen = strlen(packetData);
        ChatPacket packet;
        packet.event_id(1);
        packet.sequence_index(1);
        packet.body_length(packetDataLen);
        packet.encode_header();
        memcpy(packet.body(), packetData, strlen(packetData));
        packet.decode_header();

        auto event = CoreUtility::eventFromPacket(packet);
        auto json = QJsonDocument::fromJson(packetData).object();

        EXPECT_STREQ(event.message.message.toStdString().data(), json["message"].toString().toStdString().data());
        EXPECT_EQ(event.message.isFromMe, false) << "by default";
        EXPECT_STREQ(event.user.toStdString().data(), json["user"].toString().toStdString().data());
        EXPECT_EQ(event.type, ChatEvent::PARTICIPANT_MESSAGE);
        EXPECT_EQ(event.packetCount, 0) << "it is not a file event";
    }

    TEST(FileManager, Trivial) {

    }
}