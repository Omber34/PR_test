//
// Created by Herman on 4/16/2022.
//

#include "gtest/gtest.h"
#include "../client/ChatModel.h"
#include "QObjectTestHelper.h"
#include "../client/ChatClient.h"

#ifndef PR_TEST_CHATMODELTEST_H
#define PR_TEST_CHATMODELTEST_H

class ChatModelTest : public testing::Test {
protected:
    void SetUp() override {
        QObject::connect(&model, &ChatModel::eventAdded, [this] (){
               helper.testSlot("eventAdded");
        });

        packetSender = [] (ChatPacket packet) {
            ChatClient::getInstance().SendPacket(packet);
        };
        model.setUser("Taras");
    }

    void TearDown() override {
    }

    ChatModel model;
    QObjectTestHelper helper;
    std::deque<ChatPacket> sentPackets;
};

#endif //PR_TEST_CHATMODELTEST_H
