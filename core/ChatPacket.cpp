//
// Created by Herman on 1/17/2022.
//

#include "ChatPacket.h"

const char *ChatPacket::data() const {
    return data_;
}

void ChatPacket::encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}

bool ChatPacket::decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::stoi(header);
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
{
}
