//
// Created by Herman on 4/21/2022.
//

#include "Room.h"

#include <utility>

namespace core
{
  QString Room::getName() const
  {
    return name;
  }

  QString Room::getPasswordHash() const
  {
    return passwordHash;
  }

  bool Room::hasPassword() const
  {
    return passwordHash.size() > 0;
  }

  Room::Room(QString name, QString passwordHash) : name(std::move(name)), passwordHash(std::move(passwordHash))
  {}

  void Room::addUser(const UserPtr &user)
  {
    users.push_back(user);
  }
}
