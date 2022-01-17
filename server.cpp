//
// Created by Herman on 1/17/2022.
//

#include <iostream>
#include <boost/asio.hpp>
#include "ChatServer.h"

int main(int argc, char ** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        std::vector<std::string> ports;
        for (size_t i = 1; i < argc; ++i){
            ports.emplace_back(argv[i]);
        }

        ChatServer chatServer(ports);
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}