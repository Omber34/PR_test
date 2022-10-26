//
// Created by Herman on 1/17/2022.
//

#include "ChatPacket.h"

namespace core
{
  const uint8_t *ChatPacket::data() const
  {
    return data_.data();
  }

  void ChatPacket::encode_header()
  {
    reinterpret_cast<uint32_t *>(data_.data())[0] = event_id_;
    reinterpret_cast<uint32_t *>(data_.data())[1] = sequence_index_;
    reinterpret_cast<uint32_t *>(data_.data())[2] = body_length_;
  }

  bool ChatPacket::decode_header()
  {
    event_id_ = reinterpret_cast<uint32_t *>(data_.data())[0];
    sequence_index_ = reinterpret_cast<uint32_t *>(data_.data())[1];
    body_length_ = reinterpret_cast<uint32_t *>(data_.data())[2];
    if (body_length_ > max_body_length)
    {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void ChatPacket::body_length(uint32_t new_length)
  {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
      body_length_ = max_body_length;
  }

  uint32_t ChatPacket::body_length() const
  {
    return body_length_;
  }

  uint8_t *ChatPacket::body()
  {
    return data_.data() + header_length;
  }

  const uint8_t *ChatPacket::body() const
  {
    return data_.data() + header_length;
  }

  std::size_t ChatPacket::length() const
  {
    return header_length + body_length_;
  }

  uint8_t *ChatPacket::data()
  {
    return data_.data();
  }

  ChatPacket::ChatPacket()
          : body_length_(0), sequence_index_(0), event_id_(0)
  {
    data_.resize(ChatPacket::max_body_length + ChatPacket::header_length);
  }

  void ChatPacket::sequence_index(uint32_t new_sequence)
  {
    sequence_index_ = new_sequence;
  }

  uint32_t ChatPacket::sequence_index() const
  {
    return sequence_index_;
  }

  void ChatPacket::event_id(uint32_t new_id)
  {
    event_id_ = new_id;
  }

  uint32_t ChatPacket::event_id() const
  {
    return event_id_;
  }

  ChatPacket::ChatPacket(ChatPacket &&other)
  : data_(std::move(other.data_))
  , event_id_(other.event_id_)
  , sequence_index_(other.sequence_index_)
  , body_length_(other.body_length_)
  {
    other.data_.resize(ChatPacket::max_body_length + ChatPacket::header_length);
  }

  ChatPacket &ChatPacket::operator=(ChatPacket &&other)
  {
    data_ = std::move(other.data_);
    event_id_= other.event_id_;
    sequence_index_= other.sequence_index_;
    body_length_= other.body_length_;

    other.data_.resize(ChatPacket::max_body_length + ChatPacket::header_length);
    return *this;
  }
}