#ifndef CHECKUSER_H
#define CHECKUSER_H

#include <QObject>
#include <memory>
#include <iostream>

enum UserType
{
    Admin = 0,
    Super = 1,
};

class CheckUser : public QObject
{
    Q_OBJECT
public:
    static CheckUser *Instance();
    explicit CheckUser(QObject *parent = 0);
    ~CheckUser();
    UserType getUser();
    void setUser(UserType user);

private:
    static std::unique_ptr<CheckUser> m_instance;
    UserType m_user;
};

#endif // CHECKUSER_H
