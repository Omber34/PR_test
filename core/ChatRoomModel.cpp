//
// Created by Herman on 5/22/2022.
//

#include <fmt/format.h>
#include "ChatRoomModel.h"

namespace core
{
  namespace
  {
    template <typename collection>
    auto findImpl(collection& rooms, const QString& roomName)
    {
      auto it = std::find_if(rooms.begin(), rooms.end(),
                             [&roomName](const RoomPtr &room)
                             {
                               return room->getName() == roomName;
                             });

      if (it == rooms.end())
        throw std::logic_error(fmt::format("Room with name {} does not exists", roomName.toStdString().c_str()));

      return it;
    }
  }

  ChatRoomModel::ChatRoomModel(QObject *parent)
          : QAbstractListModel(parent) { }

  QHash<int, QByteArray> ChatRoomModel::roleNames() const
  {
    QHash<int, QByteArray> hash;
    hash.insert(RoomName, "roomName");
    hash.insert(HasPassword, "hasPassword");

    return hash;
  }

  int ChatRoomModel::rowCount(const QModelIndex &parent) const
  {
    if (parent.isValid())
      return 0;

    return rooms.size();
  }

  QVariant ChatRoomModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid())
      return QVariant();

    switch (role)
    {
      case eRoomPtr:
        return QVariant::fromValue(rooms[index.row()]);
      case RoomName:
        return QVariant::fromValue(rooms[index.row()]->getName());
      case HasPassword:
        return QVariant::fromValue(rooms[index.row()]->hasPassword());
    }

    return QVariant();
  }

  RoomPtr ChatRoomModel::CreateRoom(const core::CreateRoom &newRoom)
  {
    QString password = "123";

    if (newRoom.password.has_value())
      password = newRoom.password.value();

    auto room = std::make_shared<Room>(newRoom.name, password);

    beginInsertRows(QModelIndex(), rooms.size(), rooms.size());
    rooms.push_back(room);
    endInsertRows();

    return room;
  }

  QModelIndex ChatRoomModel::GetRoomByName(const QString &roomName) const
  {
    auto it = findRoom(roomName);
    return index(it - rooms.begin(), 0);
  }

  void ChatRoomModel::DeleteRoom(const QString &roomName)
  {
    auto it = findRoom(roomName);

    beginRemoveRows(QModelIndex(), it - rooms.begin(), it - rooms.begin());
    rooms.erase(it);
    endRemoveRows();
  }

  std::vector<RoomPtr>::iterator ChatRoomModel::findRoom(const QString &roomName)
  {
    return findImpl(rooms, roomName);
  }

  std::vector<RoomPtr>::const_iterator ChatRoomModel::findRoom(const QString &roomName) const
  {
    return findImpl(rooms, roomName);
  }
}