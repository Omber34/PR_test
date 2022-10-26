//
// Created by Herman on 4/21/2022.
//

#ifndef PR_TEST_ROOM_H
#define PR_TEST_ROOM_H

#include <QString>
#include <QCryptographicHash>
#include <vector>
#include "User.h"

#include <optional>

namespace core
{
  class Room
  {
  public:
    Room() = default;
    Room(const Room& other) = default;
    Room& operator=(const Room& other) = default;
    Room(Room&& other) = default;
    Room& operator=(Room&& other) = default;
    ~Room() = default;

    Room(QString name, QString passwordHash);

    QString getName() const;

    QString getPasswordHash() const;

    bool hasPassword() const;

    void addUser(const UserPtr &user);

    std::vector<UserPtr> users;

  private:
    QString name;
    QString passwordHash;
  };

  struct CreateRoom
  {
    QString name;
    std::optional<QString> password;
  };

  using RoomPtr = std::shared_ptr<Room>;
}

Q_DECLARE_METATYPE(core::RoomPtr);

#endif //PR_TEST_ROOM_H
