//
// Created by Herman on 5/22/2022.
//

#include "AppEvent.h"

core::AppEventType core::UserJoinEvent::getType() const
{
  return USER_JOIN;
}

core::AppEventType core::UserJoinRoomEvent::getType() const
{
  return core::USER_JOIN_ROOM;
}

QString core::UserJoinRoomEvent::getRoomName() const
{
  return roomName;
}

QString core::UserJoinRoomEvent::getRoomPassword() const
{
  return roomPassword;
}

core::AppEventType core::UserLeaveEvent::getType() const
{
  return core::USER_LEAVE;
}
