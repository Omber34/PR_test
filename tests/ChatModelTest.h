//
// Created by Herman on 4/16/2022.
//

#include "gtest/gtest.h"
#include "../client/ChatModel.h"
#include "QObjectTestHelper.h"
#include "TestClient.h"

#ifndef PR_TEST_CHATMODELTEST_H
#define PR_TEST_CHATMODELTEST_H

class ChatModelTest : public testing::Test {
public:
    ChatModelTest()
    : model(client)
    {}

protected:
    void SetUp() override {
        QObject::connect(&model, &client::ChatModel::eventAdded, [this] (){
               helper.testSlot("eventAdded");
        });

        model.setUser(username);
    }

    void TearDown() override {
        //because test client send LEAVE event back to model, that already deleted
        QObject::disconnect(&client,0,0,0);
    }

    const char* username = "Taras";
    TestClient client;
    client::ChatModel model;
    QObjectTestHelper helper;
};

#endif //PR_TEST_CHATMODELTEST_H
