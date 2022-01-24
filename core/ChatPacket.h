//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATPACKET_H
#define PR_TEST_CHATPACKET_H

#include <deque>

#include "ChatEvent.h"

class ChatPacket {
public:
    enum { sequence_length = 4 };
    enum { body_header_length = 4 };
    enum { header_length = sequence_length + body_header_length};
    enum { max_body_length = 1024 };

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

    void sequence_index(std::size_t new_sequence);
    std::size_t sequence_index() const;


private:
    char data_[header_length + max_body_length]{};
    std::size_t body_length_;
    std::size_t sequence_index_;
};


typedef std::deque<ChatPacket> ChatPacketQueue;

#endif //PR_TEST_CHATPACKET_H
