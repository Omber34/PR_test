//
// Created by Herman on 1/17/2022.
//

#include "ChatClient.h"
#include "ClientFileManager.h"
#include "ChatClientImpl.h"
#include "CoreUtility.h"

ChatClient::ChatClient()
{
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(defaultHost, defaultPort);
    impl = std::make_unique<ChatClientImpl>(io_context, endpoints,
                                            [this] (ChatPacket&& packet) {
                                                consumePacket(std::move(packet));
    });

    ioContextThread = std::thread([this](){ io_context.run(); });

}

ChatClient::~ChatClient() {
    Disconnect();
}

void ChatClient::Disconnect() {
    impl->close();
    ioContextThread.join();
}

ChatClient& ChatClient::getInstance() {
    static ChatClient client;
    return client;
}

void ChatClient::consumePacket(ChatPacket packet) {
    auto event = CoreUtility::eventFromPacket(packet);
    if (event.type == ChatEvent::EMPTY || event.type == ChatEvent::PARTICIPANT_SHARE_FILE) {
        if (ClientFileManager::getInstance().isDone(packet)) {
            eventReceived(CoreUtility::eventFromPacket(ClientFileManager::getInstance().getDone(packet)));
        }
    }
    if (event.type == ChatEvent::EventType::PARTICIPANT_SHARE_FILE) {
        event.type = ChatEvent::EventType::PARTICIPANT_FILE;
        event.message.message = QString::fromStdString(ClientFileManager::getInstance().getDownloadFilename(event));
    }
    eventReceived(event);
}

void ChatClient::SendEvent(ChatEvent event) {
    if (event.type == ChatEvent::PARTICIPANT_SHARE_FILE) {
        auto file = CoreUtility::filePacketFromEvent(event);
        for (auto &&packet : file.packets)
            impl->write(packet);
        return;
    }
    impl->write(CoreUtility::packetFromEvent(event));
}
