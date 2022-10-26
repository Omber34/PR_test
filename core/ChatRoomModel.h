//
// Created by Herman on 5/22/2022.
//

#pragma once

#include <QAbstractListModel>
#include "Room.h"
#include "User.h"

namespace core
{
  class ChatRoomModel : public QAbstractListModel
  {
  Q_OBJECT
  public:
    enum
    {
      eRoomPtr = Qt::UserRole + 1,
      RoomName,
      HasPassword,
    };

    explicit ChatRoomModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    RoomPtr CreateRoom(const CreateRoom &newRoom);
    QModelIndex GetRoomByName(const QString &roomName) const;
    void DeleteRoom(const QString &roomName);

  private:
    std::vector<RoomPtr>::iterator findRoom(const QString &roomName);
    std::vector<RoomPtr>::const_iterator findRoom(const QString &roomName) const;

    std::vector<RoomPtr> rooms;
  };

  using ChatRoomModelPtr = std::shared_ptr<ChatRoomModel>;
}



