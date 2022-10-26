//
// Created by Herman on 4/21/2022.
//

#include "ChatAppModel.h"

namespace core
{
  const ChatRoomModelPtr ChatAppModel::getRoomModel() const
  {
    return roomModel;
  }

  const ChatUserModelPtr ChatAppModel::getUserModel() const
  {
    return userModel;
  }

  ChatAppModel::ChatAppModel(QObject *parent)
  : QObject(parent)
  , roomModel(std::make_shared<ChatRoomModel>())
  , userModel(std::make_shared<ChatUserModel>())
  {

  }

  void ChatAppModel::moveUserToRoom(const QString &userName, const QString &roomName)
  {
    const auto user = userModel->GetUserByName(userName).data(ChatUserModel::eUserPtr).value<UserPtr>();
    auto roomIdx = roomModel->GetRoomByName(roomName);
    auto room = roomIdx.data(ChatRoomModel::eRoomPtr).value<RoomPtr>();
    room->addUser(user);
    roomModel->dataChanged(roomIdx, roomIdx);
    emit userMovedToRoom(user, room);
  }

  void ChatAppModel::PublishRoom(const RoomPtr &room)
  {

  }
}
