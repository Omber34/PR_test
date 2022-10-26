//
// Created by Herman on 5/22/2022.
//

#pragma once

#include <QString>
#include <memory>

namespace core
{
  enum AppEventType
  {
    EMPTY = -1,
    USER_JOIN,
    USER_JOIN_ROOM,
    USER_LEAVE,
  };

  QString eventTypeToString(AppEventType type);

  class AppEvent
  {
  public:
    AppEvent() = delete;

    AppEvent(const QString &username)
            : userName(username) { }

    AppEvent(const AppEvent &other) = default;
    AppEvent &operator=(const AppEvent &other) = default;
    AppEvent(AppEvent &&other) = default;
    AppEvent &operator=(AppEvent &&other) = default;

    QString getUserName() const { return userName; }

    virtual AppEventType getType() const = 0;
  private:
    QString userName;
  };

  class UserJoinEvent : public AppEvent
  {
  public:
    UserJoinEvent(const QString &username)
            : AppEvent(username) { }

    AppEventType getType() const override;
  };

  class UserJoinRoomEvent : public AppEvent
  {
  public:
    UserJoinRoomEvent(const QString &username, const QString &roomName, const QString &roomPassword)
            : AppEvent(username)
              , roomName(roomName)
              , roomPassword(roomPassword) { }

    AppEventType getType() const override;
    QString getRoomName() const;
    QString getRoomPassword() const;

  private:
    QString roomName;
    QString roomPassword;
  };

  class UserLeaveEvent : public AppEvent
  {
  public:
    UserLeaveEvent(const QString &username)
            : AppEvent(username) { }

    AppEventType getType() const override;
  };

  using AppEventPtr = std::shared_ptr<AppEvent>;
}



