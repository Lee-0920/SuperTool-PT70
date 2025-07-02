#include "checkUser.h"
#include <sys/types.h>

std::unique_ptr<CheckUser> CheckUser::m_instance(nullptr);

CheckUser::CheckUser(QObject *parent) : QObject(parent)
{
    m_user = UserType::Admin;
}

CheckUser *CheckUser::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new CheckUser);
    }
    return m_instance.get();
}

UserType CheckUser::getUser()
{
    return m_user;
}

void CheckUser::setUser(UserType user)
{
    m_user = user;
}

CheckUser::~CheckUser()
{

}

