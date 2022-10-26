//
// Created by Herman on 1/17/2022.
//

#include "ChatClient.h"
#include "ChatClientImpl.h"
#include "CoreUtility.h"

namespace client
{
  ChatClient::ChatClient()
  {
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(core::DefaultHost, core::DefaultPort);
    impl = std::make_unique<ChatClientImpl>(io_context, endpoints,
                                            [this](core::ChatPacket &&packet)
                                            {
                                              consumePacket(std::move(packet));
                                            });

    ioContextThread = std::thread([this]() { io_context.run(); });
  }

  ChatClient::~ChatClient()
  {
    Disconnect();
  }

  void ChatClient::Disconnect()
  {
    impl->close();
    ioContextThread.join();
  }

  void ChatClient::consumePacket(core::ChatPacket packet)
  {
    auto event = core::CoreUtility::eventFromPacket(packet);
    if (event.type == core::ChatEvent::EMPTY || event.type == core::ChatEvent::PARTICIPANT_SHARE_FILE)
    {
      auto optFileEvent = fileManager.process(std::move(packet));
      if (optFileEvent)
      {
        eventReceived(core::CoreUtility::eventFromPacket(*optFileEvent));
      }
    }

    if (event.type == core::ChatEvent::EventType::PARTICIPANT_SHARE_FILE)
    {
      event.type = core::ChatEvent::EventType::PARTICIPANT_FILE;
      event.message.message = QString::fromStdString(fileManager.getDownloadFilename(event));
    }

    eventReceived(event);
  }

  void ChatClient::SendEvent(core::ChatEvent event)
  {
    if (event.type == core::ChatEvent::PARTICIPANT_SHARE_FILE)
    {
      auto file = core::CoreUtility::filePacketFromEvent(event);
      for (auto &&packet : file.packets)
        impl->write(std::move(packet));
      return;
    }

    impl->write(core::CoreUtility::packetFromEvent(event));
  }
}
