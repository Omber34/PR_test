//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATPACKET_H
#define PR_TEST_CHATPACKET_H
#include "ChatEvent.h"

class ChatPacket {
public:
    enum { header_length = 4 };
    enum { max_body_length = 512 };

    ChatPacket();

    const char* data() const;

    char* data();

    std::size_t length() const;

    const char* body() const;

    char* body();

    std::size_t body_length() const;

    void body_length(std::size_t new_length);

    bool decode_header();

    void encode_header();

private:
    char data_[header_length + max_body_length]{};
    std::size_t body_length_;
};


#endif //PR_TEST_CHATPACKET_H
