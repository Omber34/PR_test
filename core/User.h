//
// Created by Herman on 4/21/2022.
//

#ifndef PR_TEST_USER_H
#define PR_TEST_USER_H

#include <QMetaType>
#include <QString>
#include <memory>

namespace core
{
  class User
  {
  public:
    User() = default;
    User(const User& other) = default;
    User& operator=(const User& other) = default;
    User(User&& other) = default;
    User& operator=(User&& other) = default;

    User(QString userId);

    QString getUserName() const;

    ~User() = default;

  private:
    QString userName;
  };

  struct CreateUser
  {
    QString userName;
  };

  using UserPtr = std::shared_ptr<User>;

}
Q_DECLARE_METATYPE(core::UserPtr);


#endif //PR_TEST_USER_H
