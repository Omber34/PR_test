//
// Created by Herman on 4/16/2022.
//


#include "gtest/gtest.h"
#include "ChatModelTest.h"
#include "../client/ChatClient.h"

TEST(ClientTest, Trivial) {
    EXPECT_NO_THROW(ChatClient::getInstance());
}

TEST_F(ChatModelTest, Trivial) {
    model.Greetings();



}
