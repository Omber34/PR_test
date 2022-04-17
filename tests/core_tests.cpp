//
// Created by Herman on 4/9/2022.
//

#include "gtest/gtest.h"
#include "CoreTest.h"
#include <sstream>
#include <QJsonDocument>
#include <QJsonObject>
#include "CoreUtility.h"
#include "FileManager.h"

namespace {
    TEST_F(CoreTest, TrivialMethod) {
        auto packetData = testMessagePacketData.data() + ChatPacket::header_length;
        auto packetDataLen = strlen(packetData);
        ChatPacket packet;
        packet.event_id(1);
        packet.sequence_index(1);
        packet.body_length(packetDataLen);
        packet.encode_header();
        memcpy(packet.body(), packetData, strlen(packetData) + 1);
        packet.decode_header();

        EXPECT_EQ(packet.event_id(), 1U);
        EXPECT_EQ(packet.sequence_index(), 1U);
        EXPECT_EQ(packet.body_length(), packetDataLen);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, packet.body_length()) == 0);
    }

    TEST_F(CoreTest, MessageFromEvent) {
        auto packetData = testMessagePacketData.data() + ChatPacket::header_length;
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
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, packet.body_length()) == 0);
    }

    TEST_F(CoreTest, JoinFromEvent) {
        auto packetData = testJoinPacketData.data() + ChatPacket::header_length;
        auto packetDataLen = strlen(packetData);
        auto json = QJsonDocument::fromJson(packetData).object();
        ChatEvent event;
        event.user = json["user"].toString();
        event.type = ChatEvent::PARTICIPANT_JOIN;

        auto packet = CoreUtility::packetFromEvent(event);
        EXPECT_EQ(packet.event_id(), 0);
        EXPECT_EQ(packet.sequence_index(), 0);
        EXPECT_EQ(packet.body_length(), packetDataLen);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, strlen(packetData)) == 0);
    }

    TEST_F(CoreTest, LeaveFromEvent) {
        auto packetData = testLeavePacketData.data() + ChatPacket::header_length;
        auto packetDataLen = strlen(packetData);
        auto json = QJsonDocument::fromJson(packetData).object();
        ChatEvent event;
        event.user = json["user"].toString();
        event.type = ChatEvent::PARTICIPANT_LEAVE;

        auto packet = CoreUtility::packetFromEvent(event);
        EXPECT_EQ(packet.event_id(), 0);
        EXPECT_EQ(packet.sequence_index(), 0);
        EXPECT_EQ(packet.body_length(), packetDataLen);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char*>(packet.body()), packetData, strlen(packetData)) == 0);
    }

    TEST_F(CoreTest, FromPacket) {
        auto packetData = testMessagePacketData.data() + ChatPacket::header_length;
        auto packetDataLen = strlen(packetData);
        ChatPacket packet;
        packet.event_id(1);
        packet.sequence_index(1);
        packet.body_length(packetDataLen);
        packet.encode_header();
        memcpy(packet.body(), packetData, strlen(packetData) + 1);
        packet.decode_header();

        auto event = CoreUtility::eventFromPacket(packet);
        auto json = QJsonDocument::fromJson(packetData).object();

        EXPECT_STREQ(event.message.message.toStdString().data(), json["message"].toString().toStdString().data());
        EXPECT_EQ(event.message.isFromMe, false) << "by default";
        EXPECT_STREQ(event.user.toStdString().data(), json["user"].toString().toStdString().data());
        EXPECT_EQ(event.type, ChatEvent::PARTICIPANT_MESSAGE);
        EXPECT_EQ(event.packetCount, 0) << "it is not a file event";
    }

    TEST_F(CoreTest, Trivial) {
        char testFileData[] =   "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z"
                            "a b c d e f g h i j k l m n o p q r s t u v w x y z\0";

        const size_t testFileDataSize = strlen(testFileData);
        auto packet2p = testFileData + ChatPacket::max_body_length;
        std::stringstream testFile;
        testFile.write(testFileData, testFileDataSize);

        auto filePacket1 = FileManager::loadFileToFilePacket(testFile);
        EXPECT_EQ(filePacket1.packets.size(), 2);
        EXPECT_TRUE(strncmp(reinterpret_cast<const char *>(filePacket1.packets.begin()->body()), testFileData, ChatPacket::max_body_length) == 0);
        EXPECT_STREQ(reinterpret_cast<const char *>((++filePacket1.packets.begin())->body()), packet2p);
        EXPECT_EQ(testFileDataSize - filePacket1.packets.begin()->body_length(), (++filePacket1.packets.begin())->body_length());


        std::stringstream savedFile;
        FileManager::saveFilePacketToDisk(savedFile, filePacket1);

        EXPECT_STREQ(savedFile.str().c_str(), testFileData);
    }

    TEST(FileManager, Trivial) {

    }
}