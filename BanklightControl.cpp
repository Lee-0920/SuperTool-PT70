/**
 * @file
 * @brief 屏幕亮度控制。
 * @details
 * @version 1.0.0
 * @author xingfan
 * @date 2016/10/20
 */

#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
#include <stdio.h>
#include <QDebug>
#include <QString>
#include "BanklightControl.h"
#include <QFile>

#ifdef _CS_ARM_LINUX
    #include <sys/ioctl.h>
    #include <linux/kernel.h>
#endif

namespace System
{
namespace Screen
{


#define SYSFS_BACKLIGHT_BRIGHTNESS   "/sys/class/backlight/pwm-backlight/brightness" //设置屏幕亮度的文件
#define SYSFS_BACKLIGHT_POWER       "/sys/class/backlight/pwm-backlight/bl_power"//设置屏幕电源开关的文件

/**
 * @brief 设置屏幕亮度，更改文件brightness里的数值便可更改亮度
 * @param valve 亮度， 1 - 100
 * @return 设置结果，true为成功
 */
bool BanklightControl::SetBrightness(unsigned char valve)
{
    bool ret = false;
    QFile file(SYSFS_BACKLIGHT_BRIGHTNESS);
    if (file.exists())
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            if(valve > 0 && valve <= 100)//参数校验
             {
                QString text = QString::number(valve);
                file.write((const char *)text.toLatin1().data(), text.length());
                ret = true;
             }
            file.close();
        }
        else
        {
            qDebug("file open false: %s", SYSFS_BACKLIGHT_BRIGHTNESS);
        }
    }
    else
    {
        qDebug("file not exists: %s", SYSFS_BACKLIGHT_BRIGHTNESS);
    }
    return ret;

    return ret;
}

/**
 * @brief 设置屏幕背光灯电源的开关，更改文件bl_power里的数值便可更改开关
 * @param valve 0 为打开 ， 非零为关闭
 * @return 设置结果，true为成功
 */
bool BanklightControl::SetPower(unsigned char valve)
{
    bool ret = false;
    QFile file(SYSFS_BACKLIGHT_POWER);
    if (file.exists())
    {
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            if(valve <= 100)//参数校验
             {
                QString text = QString::number(valve);
                file.write((const char *)text.toLatin1().data(), text.length());
                ret = true;
             }
            file.close();
        }
        else
        {
            qDebug("file open false: %s", SYSFS_BACKLIGHT_POWER);
        }
    }
    else
    {
        qDebug("file not exists: %s", SYSFS_BACKLIGHT_POWER);
    }
    return ret;
}

}
}


