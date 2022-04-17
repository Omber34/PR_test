//
// Created by Herman on 4/17/2022.
//

#ifndef PR_TEST_CORETEST_H
#define PR_TEST_CORETEST_H
#include "gtest/gtest.h"
#include "ChatPacket.h"
#include <memory>

class CoreTest : public testing::Test {
protected:
    std::vector<char> createNewPacket(const char* packetBodyData){
        std::vector<char> packetData;
        auto size = strlen(packetBodyData) + ChatPacket::header_length + 1;
        packetData.resize(size);
        uint32_t *cvtPacketData = reinterpret_cast<uint32_t *>(packetData.data());
        cvtPacketData[0] = 0;
        cvtPacketData[1] = 0;
        cvtPacketData[2] = strlen(packetBodyData);
        memcpy(packetData.data() + ChatPacket::header_length, packetBodyData, strlen(packetBodyData));
        packetData[size - 1] = '\0';
        return std::move(packetData);
    }

    void SetUp() override {
        testLeavePacketData = createNewPacket(R"({"type":3,"user":"Taras"})");
        testJoinPacketData = createNewPacket(R"({"type":2,"user":"Taras"})");
        testMessagePacketData = createNewPacket(R"({"message":"vstavaite, kaidany porvite i vrazhou zlou kroviu zemly okropite","type":0,"user":"Taras"})");
    }

    std::vector<char> testLeavePacketData;
    std::vector<char> testJoinPacketData;
    std::vector<char> testMessagePacketData;
};


#endif //PR_TEST_CORETEST_H
