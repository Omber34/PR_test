//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATPACKET_H
#define PR_TEST_CHATPACKET_H

#include <deque>

#include "ChatEvent.h"

class ChatPacket {
public:
    enum { event_id_length = 4 };
    enum { sequence_length = 4 };
    enum { payload_size_length = 4 };
    enum { header_length = event_id_length + sequence_length + payload_size_length };
    enum { max_body_length = 1024 };

    ChatPacket();
    ChatPacket(const ChatPacket& other);
    ChatPacket& operator=(const ChatPacket& other);
    ChatPacket(ChatPacket&& other) = default;
    ChatPacket& operator=(ChatPacket&& other) = default;

    const uint8_t * data() const;

    uint8_t * data();

    std::size_t length() const;

    const uint8_t * body() const;

    uint8_t * body();

    uint32_t body_length() const;

    void body_length(uint32_t new_length);

    bool decode_header();

    void encode_header();

    void sequence_index(uint32_t new_sequence);
    uint32_t sequence_index() const;

    void event_id(uint32_t new_id);
    uint32_t event_id() const;

private:
    //State
    std::unique_ptr<uint8_t[]> data_;
    uint32_t body_length_;
    uint32_t sequence_index_;
    uint32_t event_id_;
};


typedef std::deque<ChatPacket> ChatPacketQueue;

#endif //PR_TEST_CHATPACKET_H
