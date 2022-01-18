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
    while (std::cin.getline(line, ChatPacket::max_body_length + 1))
    {
        ChatPacket msg;
        msg.body_length(std::strlen(line));
        std::memcpy(msg.body(), line, msg.body_length());
        msg.encode_header();
        client.SendPacket(msg);
    }

    client.Disconnect();
    return 0;
}
