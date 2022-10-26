//
// Created by Herman on 5/22/2022.
//

#pragma once

#include <QAbstractListModel>
#include "User.h"

namespace core
{
  class ChatUserModel : public QAbstractListModel
  {
  Q_OBJECT
  public:
    enum
    {
      eUserPtr = Qt::UserRole + 1,
      UserId,
    };

    explicit ChatUserModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;


    UserPtr CreateUser(const CreateUser &newUser);
    void JoinUser(const UserPtr &user);
    QModelIndex GetUserByName(const QString &userID) const;
    void DeleteUser(const QString &userID);


  private:
    std::vector<UserPtr>::iterator findUser(const QString &userID);
    std::vector<UserPtr>::const_iterator findUser(const QString &userID) const;

    std::vector<UserPtr> users;
  };

  using ChatUserModelPtr = std::shared_ptr<ChatUserModel>;
}



