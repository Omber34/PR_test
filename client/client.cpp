#include <iostream>
#include "ChatClient.h"

int main(int argc, char ** argv) {
    std::string host = "127.0.0.1", port = "4026";
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    }
    ChatClient client(host, port);

    char line[ChatPacket::max_body_length + 1];

    std::cin.getline(line, ChatPacket::max_body_length + 1);
    client.setUsername(line);

    while (std::cin.getline(line, ChatPacket::max_body_length + 1))
    {
        ChatEvent event;
        event.user = QString::fromStdString(client.getUsername());
        event.message = ChatMessage{QString(line), true};
        event.type = ChatEvent::EventType::PARTICIPANT_MESSAGE;
        client.SendPacket(PacketEventTransform::packetFromEvent(event));
    }

    client.Disconnect();
    return 0;
}
