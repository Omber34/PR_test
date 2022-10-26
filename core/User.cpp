//
// Created by Herman on 4/21/2022.
//

#include "User.h"

#include <utility>

namespace core
{
  QString User::getUserName() const
  {
    return userName;
  }

  User::User(QString userId) : userName(std::move(userId))
  {}
}
