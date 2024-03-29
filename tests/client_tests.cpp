//
// Created by Herman on 4/16/2022.
//
#include <FileManager.h>
#include "gtest/gtest.h"
#include "ChatModelTest.h"
#include "FileManagerTest.h"
#include "../client/ChatClient.h"

using namespace client;
using namespace core;

TEST_F(ChatModelTest, Trivial)
{
  EXPECT_EQ(helper.counters["eventAdded"], 1);
  EXPECT_EQ(model.rowCount(), 1);

  auto event = model.data(model.index(0), ChatModel::eEventRole).value<ChatEvent>();
  EXPECT_EQ(event.type, ChatEvent::PARTICIPANT_JOIN);
  EXPECT_STREQ(event.user.toStdString().c_str(), username);
}

TEST_F(ChatModelTest, SendMessage)
{
  const char *messageText = "SLAVA UKRAINE";
  model.sendMessage(messageText);

  EXPECT_EQ(helper.counters["eventAdded"], 2);
  EXPECT_EQ(model.rowCount(), 2);

  auto event = model.data(model.index(1), ChatModel::eEventRole).value<ChatEvent>();
  EXPECT_EQ(event.type, ChatEvent::PARTICIPANT_MESSAGE);
  EXPECT_STREQ(event.user.toStdString().c_str(), username);
  EXPECT_STREQ(event.message.message.toStdString().c_str(), messageText);
  EXPECT_TRUE(event.message.isFromMe);
}

TEST_F(ChatModelTest, SendFile)
{
  const char *fileName = "SLAVA_UKRAINE.txt";
  model.sendFile(fileName);

  EXPECT_EQ(helper.counters["eventAdded"], 2);
  EXPECT_EQ(model.rowCount(), 2);

  auto event = model.data(model.index(1), ChatModel::eEventRole).value<ChatEvent>();
  EXPECT_EQ(event.type, ChatEvent::PARTICIPANT_SHARE_FILE);
  EXPECT_STREQ(event.user.toStdString().c_str(), username);
  EXPECT_STREQ(event.message.message.toStdString().c_str(), fileName);
  EXPECT_TRUE(event.message.isFromMe);
}

TEST_F(ChatModelTest, AddEvent)
{
  const char *anotherUsername = "Vasil";
  ChatEvent joinEvent;
  joinEvent.user = anotherUsername;
  joinEvent.type = ChatEvent::PARTICIPANT_JOIN;
  model.addEvent(joinEvent);

  ChatEvent messageEvent;
  messageEvent.type = ChatEvent::PARTICIPANT_MESSAGE;
  messageEvent.message = { "HEROYAM SLAVA", true };
  messageEvent.user = anotherUsername;
  model.addEvent(messageEvent);

  ChatEvent leaveEvent;
  leaveEvent.user = anotherUsername;
  leaveEvent.type = ChatEvent::PARTICIPANT_LEAVE;
  model.addEvent(messageEvent);

  EXPECT_EQ(helper.counters["eventAdded"], 4);
  EXPECT_EQ(model.rowCount(), 4);
}

TEST_F(FileManagerTest, Trivial)
{
  ClientFileManager manager;
  std::stringstream testFile;
  testFile.write(testTxtFileData, testTxtFileDataSize);

  auto filePacket = FileManager::loadFileToFilePacket(testFile);
  std::optional<ChatPacket> result;
  for (auto packet : filePacket.packets)
    result = manager.process(std::move(packet));

  EXPECT_TRUE(result.has_value());
}
