//
// Created by Herman on 1/17/2022.
//

#include "ChatPacket.h"

const char *ChatPacket::data() const {
    return data_;
}

void ChatPacket::encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d%4d%4d", static_cast<int>(event_id_), static_cast<int>(sequence_index_), static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}

bool ChatPacket::decode_header() {
    char event_id_header[event_id_length + 1] = "";
    char sequence_header[sequence_length + 1] = "";
    char body_header_length_[payload_size_length + 1] = "";
    std::strncat(event_id_header, data_, event_id_length);
    std::strncat(sequence_header, data_ + event_id_length, sequence_length);
    std::strncat(body_header_length_, data_ + event_id_length + sequence_length, payload_size_length);
    event_id(std::stoi(event_id_header));
    sequence_index_ = std::stoi(sequence_header);
    body_length_ = std::stoi(body_header_length_);
    if (body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }
    return true;
}

void ChatPacket::body_length(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

std::size_t ChatPacket::body_length() const {
    return body_length_;
}

char *ChatPacket::body() {
    return data_ + header_length;
}

const char *ChatPacket::body() const {
    return data_ + header_length;
}

std::size_t ChatPacket::length() const {
    return header_length + body_length_;
}

char *ChatPacket::data() {
    return data_;
}

ChatPacket::ChatPacket()
    : body_length_(0)
    , sequence_index_(0)
    , event_id_(0)
{
}

void ChatPacket::sequence_index(std::size_t new_sequence) {
    sequence_index_ = new_sequence;
}

std::size_t ChatPacket::sequence_index() const {
    return sequence_index_;
}

void ChatPacket::event_id(std::size_t new_id) {
    event_id_ = new_id;
}

std::size_t ChatPacket::event_id() const {
    return event_id_;
}
