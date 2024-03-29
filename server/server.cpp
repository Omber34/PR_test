//
// Created by Herman on 1/17/2022.
//

#include <iostream>
#include <boost/asio.hpp>
#include "ChatServer.h"

int main(int argc, char ** argv) {

    std::vector<std::string> ports;

    if (argc < 2) {
        ports.emplace_back("4026");
    }
    else {
        for (size_t i = 1; i < argc; ++i) {
            ports.emplace_back(argv[i]);
        }
    }
    server::ChatServer chatServer;


    return 0;
}