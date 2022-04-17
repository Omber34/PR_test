//
// Created by Herman on 4/17/2022.
//

#ifndef PR_TEST_CLIENTFILEMANAGERTEST_H
#define PR_TEST_CLIENTFILEMANAGERTEST_H

#include "gtest/gtest.h"
#include "../client/ClientFileManager.h"
#include "QObjectTestHelper.h"

class ClientFileManagerTest : public testing::Test {
protected:
    void SetUp() override {
        QObject::connect(&model, &ChatModel::eventAdded, [this] (){
            helper.testSlot("eventAdded");
        });

        model.setUser(username);
    }

    void TearDown() override {
        //because test client send LEAVE event back to model, that already deleted
        QObject::disconnect(&client,0,0,0);
    }

    QObjectTestHelper helper;
};

#endif //PR_TEST_CLIENTFILEMANAGERTEST_H
