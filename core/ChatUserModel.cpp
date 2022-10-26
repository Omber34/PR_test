//
// Created by Herman on 5/22/2022.
//

#include <stdexcept>
#include "ChatUserModel.h"
#include <fmt/format.h>

namespace core
{
  namespace
  {
    QString GetUserId()
    {
      static uint32_t id = 0;
      return QString::number(id++);
    }

    template <typename collection>
    auto findImpl(collection& users, const QString& userID)
    {
      auto it = std::find_if(users.begin(), users.end(),
                             [&userID](const UserPtr &user)
                             {
                               return user->getUserName() == userID;
                             });

      if (it == users.end())
        throw std::logic_error(fmt::format("User with id {} does not exists", userID.toStdString().c_str()));

      return it;
    }

  }

  ChatUserModel::ChatUserModel(QObject *parent)
          : QAbstractListModel(parent) { }

  QHash<int, QByteArray> ChatUserModel::roleNames() const
  {
    QHash<int, QByteArray> hash;
    hash.insert(UserId, "UserId");

    return hash;
  }

  int ChatUserModel::rowCount(const QModelIndex &parent) const
  {
    if (parent.isValid())
      return 0;

    return users.size();
  }

  QVariant ChatUserModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid())
      return QVariant();

    switch (role)
    {
      case eUserPtr:
        return QVariant::fromValue(users[index.row()]);
      case UserId:
        return QVariant::fromValue(users[index.row()]->getUserName());
    }

    return QVariant();
  }

  std::vector<UserPtr>::const_iterator ChatUserModel::findUser(const QString &userID) const
  {
    return findImpl(users, userID);
  }

  std::vector<UserPtr>::iterator ChatUserModel::findUser(const QString &userID)
  {
    return findImpl(users, userID);
  }

  UserPtr ChatUserModel::CreateUser(const core::CreateUser &newUser)
  {
    auto user = std::make_shared<User>(newUser.userName);

    beginInsertRows(QModelIndex(), users.size(), users.size());
    users.push_back(user);
    endInsertRows();
    return user;
  }

  void ChatUserModel::JoinUser(const UserPtr &user)
  {
    beginInsertRows(QModelIndex(), users.size(), users.size());
    users.push_back(user);
    endInsertRows();
  }

  QModelIndex ChatUserModel::GetUserByName(const QString &userID) const
  {
    auto it = findUser(userID);
    return index(it - users.begin(), 0);
  }

  void ChatUserModel::DeleteUser(const QString &userID)
  {
    auto it = findUser(userID);

    beginRemoveRows(QModelIndex(), it - users.begin(), it - users.begin());
    users.erase(it);
    endRemoveRows();
  }

}