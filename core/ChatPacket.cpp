//
// Created by Herman on 1/17/2022.
//

#include "ChatPacket.h"
#include <QDebug>

const uint8_t * ChatPacket::data() const {
    return data_.get();
}

void ChatPacket::encode_header() {
    reinterpret_cast<uint32_t*>(data_.get())[0] = event_id_;
    reinterpret_cast<uint32_t*>(data_.get())[1] = sequence_index_;
    reinterpret_cast<uint32_t*>(data_.get())[2] = body_length_;
}

bool ChatPacket::decode_header() {
    event_id_ = reinterpret_cast<uint32_t*>(data_.get())[0];
    sequence_index_ = reinterpret_cast<uint32_t*>(data_.get())[1];
    body_length_ = reinterpret_cast<uint32_t*>(data_.get())[2];
    if (body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }
    return true;
}

void ChatPacket::body_length(uint32_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

uint32_t ChatPacket::body_length() const {
    return body_length_;
}

uint8_t * ChatPacket::body() {
    return data_.get() + header_length;
}

const uint8_t * ChatPacket::body() const {
    return data_.get() + header_length;
}

std::size_t ChatPacket::length() const {
    return header_length + body_length_;
}

uint8_t * ChatPacket::data() {
    return data_.get();
}

ChatPacket::ChatPacket()
    : body_length_(0)
    , sequence_index_(0)
    , event_id_(0)
    , data_(std::unique_ptr<uint8_t[]>(new uint8_t[max_body_length]))
{
}

void ChatPacket::sequence_index(uint32_t new_sequence) {
    sequence_index_ = new_sequence;
}

uint32_t ChatPacket::sequence_index() const {
    return sequence_index_;
}

void ChatPacket::event_id(uint32_t new_id) {
    event_id_ = new_id;
}

uint32_t ChatPacket::event_id() const {
    return event_id_;
}

ChatPacket::ChatPacket(const ChatPacket &other) {
    event_id_ = other.event_id_;
    sequence_index_ = other.sequence_index_;
    body_length_ = other.body_length_;
    data_.reset(new uint8_t[body_length_]);
    memcpy(data_.get(), other.data_.get(), body_length_);
}

ChatPacket &ChatPacket::operator=(const ChatPacket &other) {
    event_id_ = other.event_id_;
    sequence_index_ = other.sequence_index_;
    body_length_ = other.body_length_;
    data_.reset(new uint8_t[body_length_]);
    memcpy(data_.get(), other.data_.get(), body_length_);
    return *this;
}
