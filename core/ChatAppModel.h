//
// Created by Herman on 4/21/2022.
//

#ifndef PR_TEST_CHATAPPMODEL_H
#define PR_TEST_CHATAPPMODEL_H

#include <QAbstractListModel>
#include "ChatRoomModel.h"
#include "ChatUserModel.h"

namespace core
{
  class ChatAppModel : public QObject
  {
    Q_OBJECT
  public:

    Q_PROPERTY(ChatRoomModelPtr roomModel READ getRoomModel)
    Q_PROPERTY(ChatUserModelPtr userModel READ getUserModel)

    explicit ChatAppModel(QObject *parent = nullptr);

    const ChatRoomModelPtr getRoomModel() const;
    const ChatUserModelPtr getUserModel() const;

    Q_INVOKABLE void moveUserToRoom(const QString &userName, const QString &roomName);
    void PublishRoom(const RoomPtr& room);

signals:
    void userMovedToRoom(UserPtr user, RoomPtr room);

  private:
    ChatRoomModelPtr roomModel;
    ChatUserModelPtr userModel;

  };

  using ChatAppModelPtr = std::shared_ptr<ChatAppModel>;
}


#endif //PR_TEST_CHATAPPMODEL_H
