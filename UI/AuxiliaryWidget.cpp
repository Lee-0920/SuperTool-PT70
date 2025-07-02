#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QProcess>
#include <QString>
#include "FileOperateWidget.h"
#include "MessageDialog.h"
#include "LuipMessageDialog.h"
#include "System/AppCheck.h"
#include "Flow/FlowManager.h"
#include "SysTimeSettingDialog.h"
#include "AuxiliaryWidget.h"
#include <QDir>
#include "checkUser.h"

using namespace Flow;

namespace UI
{
AuxiliaryWidget::AuxiliaryWidget(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setPointSize(13);

    m_clearBackupButton = new QPushButton();
    m_clearBackupButton->setObjectName("brownButton");
    m_clearBackupButton->setFixedSize(200,50);
    m_clearBackupButton->setText("清空备份");
    m_clearBackupButton->setFont(font);
    connect(m_clearBackupButton, SIGNAL(clicked(bool)),  this, SLOT(ClearBackupSlot()));

    m_setDateTimeButton = new QPushButton();
    m_setDateTimeButton->setObjectName("redButton");
    m_setDateTimeButton->setFixedSize(200,50);
    m_setDateTimeButton->setText("清空仪器信息及数据");
    m_setDateTimeButton->setFont(font);
    connect(m_setDateTimeButton, SIGNAL(clicked(bool)),  this, SLOT(SetDateTimeSlot()));

    m_advancedButton = new QPushButton();
    m_advancedButton->setObjectName("brownButton");
    m_advancedButton->setFixedSize(200,50);
    m_advancedButton->setText("高级操作");
    m_advancedButton->setFont(font);
    connect(m_advancedButton, SIGNAL(clicked(bool)),  this, SLOT(AdvancedSlot()));

    m_startLuipButton = new QPushButton();
    m_startLuipButton->setObjectName("greenButton");
    m_startLuipButton->setFixedSize(200,50);
    m_startLuipButton->setText("启动应用程序");
    m_startLuipButton->setFont(font);
    connect(m_startLuipButton, SIGNAL(clicked(bool)),  this, SLOT(StartLuipSlot()));

    m_oneKeyBackupButton = new QPushButton();
    m_oneKeyBackupButton->setObjectName("brownButton");
    m_oneKeyBackupButton->setFixedSize(200,50);
    m_oneKeyBackupButton->setText("一键备份");
    m_oneKeyBackupButton->setFont(font);
    connect(m_oneKeyBackupButton, SIGNAL(clicked(bool)),  this, SLOT(OneKeyBackupSlot()));

    m_extractBackupButton = new QPushButton();
    m_extractBackupButton->setObjectName("brownButton");
    m_extractBackupButton->setFixedSize(200,50);
    m_extractBackupButton->setText("提取备份");
    m_extractBackupButton->setFont(font);
    connect(m_extractBackupButton, SIGNAL(clicked(bool)),  this, SLOT(ExtractBackupSlot()));

    m_clearDataButton = new QPushButton();
    m_clearDataButton->setObjectName("warnButton");
    m_clearDataButton->setFixedSize(200,50);
    m_clearDataButton->setText("清空数据");
    m_clearDataButton->setFont(font);
    connect(m_clearDataButton, SIGNAL(clicked(bool)),  this, SLOT(ClearDataSlot()));

    m_clearAllButton = new QPushButton();
    m_clearAllButton->setObjectName("warnButton");
    m_clearAllButton->setFixedSize(200,50);
    m_clearAllButton->setText("清空配置");
    m_clearAllButton->setFont(font);
    connect(m_clearAllButton, SIGNAL(clicked(bool)),  this, SLOT(ClearAllSlot()));

    m_copyLogTxtButton = new QPushButton();
    m_copyLogTxtButton->setObjectName("brownButton");
    m_copyLogTxtButton->setFixedSize(200,50);
    m_copyLogTxtButton->setText("导出单份日志");
    m_copyLogTxtButton->setFont(font);
    connect(m_copyLogTxtButton, SIGNAL(clicked(bool)),  this, SLOT(GetLogTxtSlot()));

    m_copyAllLogTxtButton = new QPushButton();
    m_copyAllLogTxtButton->setObjectName("brownButton");
    m_copyAllLogTxtButton->setFixedSize(200,50);
    m_copyAllLogTxtButton->setText("导出所有日志");
    m_copyAllLogTxtButton->setFont(font);
    connect(m_copyAllLogTxtButton, SIGNAL(clicked(bool)),  this, SLOT(GetAllLogTxtSlot()));

    QPushButton *clearSN;
    clearSN = new QPushButton();
    clearSN->setObjectName("warnButton");
    clearSN->setFixedSize(200,50);
    clearSN->setText("清空SN及型码");
    clearSN->setFont(font);
    connect(clearSN, SIGNAL(clicked(bool)),  this, SLOT(ClearSNSlot()));

    QPushButton *clearMeasureStatus;
    clearMeasureStatus = new QPushButton();
    clearMeasureStatus->setObjectName("brownButton");
    clearMeasureStatus->setFixedSize(200,50);
    clearMeasureStatus->setText("清空测量状态");
    clearMeasureStatus->setFont(font);
    connect(clearMeasureStatus, SIGNAL(clicked(bool)),  this, SLOT(ClearMeasureStatusSlot()));

    QPushButton *clearRemain;
    clearRemain = new QPushButton();
    clearRemain->setObjectName("brownButton");
    clearRemain->setFixedSize(200,50);
    clearRemain->setText("清空试剂状态");
    clearRemain->setFont(font);
    connect(clearRemain, SIGNAL(clicked(bool)),  this, SLOT(ClearRemainSlot()));

    QPushButton *clearHardware;
    clearHardware = new QPushButton();
    clearHardware->setObjectName("brownButton");
    clearHardware->setFixedSize(200,50);
    clearHardware->setText("更新硬件默认参数");
    clearHardware->setFont(font);
    connect(clearHardware, SIGNAL(clicked(bool)),  this, SLOT(ClearHardwareSlot()));

    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addWidget(m_oneKeyBackupButton,0,0,1,1);
    buttonLayout->addWidget(m_clearBackupButton,0,1,1,1);
    buttonLayout->addWidget(m_extractBackupButton,1,0,1,1);
    buttonLayout->addWidget(m_advancedButton,1,1,1,1);
    buttonLayout->addWidget(m_startLuipButton,2,0,1,1);
    buttonLayout->addWidget(clearHardware,2,1,1,1);
    buttonLayout->addWidget(m_copyLogTxtButton,3,0,1,1);
    buttonLayout->addWidget(m_copyAllLogTxtButton,3,1,1,1);
    buttonLayout->addWidget(m_clearAllButton,5,0,1,1);
    buttonLayout->addWidget(m_clearDataButton,5,1,1,1);
    buttonLayout->addWidget(clearRemain,4,0,1,1);
    buttonLayout->addWidget(clearMeasureStatus,4,1,1,1);
    buttonLayout->addWidget(clearSN,6,0,1,1);
    buttonLayout->addWidget(m_setDateTimeButton,6,1,1,1);
    buttonLayout->setHorizontalSpacing(50);
    buttonLayout->setVerticalSpacing(0);

    QHBoxLayout *vLayout = new QHBoxLayout();
    vLayout->addStretch();
    vLayout->addLayout(buttonLayout);
    vLayout->addStretch();
    vLayout->setSpacing(20);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();
    hLayout->setSpacing(20);
    hLayout->setContentsMargins(0, 0, 0, 0);

    if (CheckUser::Instance()->getUser() == UserType::Admin)
    {
        clearSN->hide();
        m_setDateTimeButton->hide();
    }

    this->setLayout(hLayout);
}

void AuxiliaryWidget::ClearBackupSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "删除备份文件", "是否删除全部备份文件？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QDir mbDir(SuperToolBakDir);
        if(mbDir.exists())
        {
            QString cmd = QString("rm -r ") + SuperToolBakDir;
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }
    }
}

void AuxiliaryWidget::SetDateTimeSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空信息及配置配置", "是否删除所有信息及配置文件？\n清空后可跨参数升级！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        QDir spDir("/opt/bakSP/");
        if(!spDir.exists())
        {
            QString cmd = QString("mkdir /opt/bakSP/");
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }

        system("mv /opt/luip/SP/Config/Default /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ConfigLists.ls /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ConfigModifyRecord.ls /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ProducerAuthorization.ls /opt/bakSP/");
        system("rm /opt/luip/SP/Config/* -r");
        system("mv /opt/bakSP/* /opt/luip/SP/Config/");
        system("rm /opt/bakSP -r");
        system("cp /opt/luip/CheckFile/bin/ProducerAuthorization.ls /opt/luip/SP/Config");

        system("rm /opt/LuipData/*.bin -r");
        system("rm /opt/LuipData/log.txt.* -r");
        system("rm /opt/luip/*.bin");

        system("rm /mnt/datadisk/Data -r");

        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 5,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::AdvancedSlot()
{
    QString path = "/mnt/udisk/SuperTool.sh";

    if (QFile::exists(path))
    {
        if(QProcess::startDetached(path, QStringList()))
        {
                QMessageBox::information(NULL, "高级操作", "高级操作完成！", QMessageBox::Yes);
        }
        else
        {
            QMessageBox::warning(NULL, "高级操作", "无法启动高级操作！", QMessageBox::Yes);
        }
    }
    else
    {
        QMessageBox::warning(NULL, "高级操作", "高级操作脚本不存在！", QMessageBox::Yes);
    }
}

void AuxiliaryWidget::StartLuipSlot()
{
    FlowManager::Instance()->OnStartLuip();
}

void AuxiliaryWidget::OneKeyBackupSlot()
{
#ifndef WIN32
    QDir mbDir("/opt/SuperToolBak/");
    if(!mbDir.exists())
    {
        QString cmd = QString("mkdir /opt/SuperToolBak/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }
    QDir dataDir("/opt/SuperToolBak/LuipData/");
    if(!dataDir.exists())
    {
        QString cmd = QString("mkdir /opt/SuperToolBak/LuipData/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }
    QDir binDir("/opt/SuperToolBak/bin/");
    if(!binDir.exists())
    {
        QString cmd = QString("mkdir /opt/SuperToolBak/bin/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }

    system("cp /opt/LuipData/*.bin /opt/SuperToolBak/LuipData/ -r");
    system("cp /opt/luip/*.bin /opt/SuperToolBak/bin -r");
    system("cp /mnt/datadisk/Data /opt/SuperToolBak/bin -r");
    system("cp /opt/LuipData/log.txt /opt/SuperToolBak/LuipData -r");
    system("cp /opt/luip/SP /opt/SuperToolBak/ -r");
    system("sync");
#endif
    LuipMessageDialog msg("备份中\n", 10,this,MsgStyle::NONE);
    msg.exec();
}

void AuxiliaryWidget::ExtractBackupSlot()
{
#ifndef WIN32
    QDir dir("/mnt/udisk/Updater");
    if(!dir.exists())
    {
        QMessageBox::StandardButton rb = QMessageBox::information(NULL,"提醒","请插入U盘并创建Updater文件夹",QMessageBox::Ok, QMessageBox::Ok);
        if(rb == QMessageBox::Ok)
        {
            return;
        }
    }
    QDir mbDir("/opt/SuperToolBak/");
    if(!mbDir.exists())
    {
        QString cmd = QString("mkdir /opt/SuperToolBak/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }
    system("rm /mnt/udisk/Updater/SuperToolBak.tar.gz");
    system("tar -czvf /mnt/udisk/Updater/SuperToolBak.tar.gz /opt/SuperToolBak/");
    system("sync");
#endif
    LuipMessageDialog msg("提取中\n", 10,this,MsgStyle::NONE);
    msg.exec();
}

void AuxiliaryWidget::ClearDataSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空数据", "是否删除全部数据？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        system("rm /opt/LuipData/*.bin -r");
        system("rm /opt/LuipData/log.txt.* -r");
        system("rm /opt/luip/*.bin");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 3,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::ClearAllSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空配置", "是否删除配置文件？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        QDir spDir("/opt/bakSP/");
        if(!spDir.exists())
        {
            QString cmd = QString("mkdir /opt/bakSP/");
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }

        system("mv /opt/luip/SP/Config/Default /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ConfigLists.ls /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ConfigModifyRecord.ls /opt/bakSP/");
        system("mv /opt/luip/SP/Config/ProducerAuthorization.ls /opt/bakSP/");
        system("rm /opt/luip/SP/Config/* -r");
        system("mv /opt/bakSP/* /opt/luip/SP/Config/");
        system("rm /opt/bakSP -r");
        system("cp /opt/luip/CheckFile/bin/ProducerAuthorization.ls /opt/luip/SP/Config");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 3,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::GetLogTxtSlot()
{
#ifndef WIN32
    QDir dir("/mnt/udisk/Updater");
    if(!dir.exists())
    {
        QMessageBox::StandardButton rb = QMessageBox::information(NULL,"提醒","请插入U盘并创建Updater文件夹",QMessageBox::Ok, QMessageBox::Ok);
        if(rb == QMessageBox::Ok)
        {
            return;
        }
    }
    QDir mbDir("/opt/LogBak/");
    if(!mbDir.exists())
    {
        QString cmd = QString("mkdir /opt/LogBak/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }
    system("cp /opt/LuipData/log.txt /mnt/udisk/Updater");
    system("sync");
#endif
    LuipMessageDialog msg("提取中\n", 5,this,MsgStyle::NONE);
    msg.exec();
}

void AuxiliaryWidget::GetAllLogTxtSlot()
{
#ifndef WIN32
    QDir dir("/mnt/udisk/Updater");
    if(!dir.exists())
    {
        QMessageBox::StandardButton rb = QMessageBox::information(NULL,"提醒","请插入U盘并创建Updater文件夹",QMessageBox::Ok, QMessageBox::Ok);
        if(rb == QMessageBox::Ok)
        {
            return;
        }
    }
    QDir mbDir("/opt/LogBak/");
    if(!mbDir.exists())
    {
        QString cmd = QString("mkdir /opt/LogBak/");
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }
    system("cp /opt/LuipData/log.* /opt/LogBak/");
    system("rm /mnt/udisk/Updater/LogBak.tar.gz");
    system("tar -czvf /mnt/udisk/Updater/LogBak.tar.gz /opt/LogBak/");
    system("rm -r /opt/LogBak");
    system("sync");
#endif
    LuipMessageDialog msg("提取中\n", 10,this,MsgStyle::NONE);
    msg.exec();
}

void AuxiliaryWidget::ClearSNSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空SN号", "是否删除SN号配置文件？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        system("rm /mnt/datadisk/Data -r");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 1,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::ClearMeasureStatusSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空测量状态", "是否删除测量状态文件？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        system("rm /opt/luip/SP/Config/MeasureStatus.ls");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 1,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::ClearRemainSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空试剂状态", "是否删除试剂配置文件？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        system("rm /opt/luip/SP/Config/RemainConfig.ls");
        system("rm /opt/luip/SP/Config/RemainStatus.ls");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 1,this,MsgStyle::NONE);
        msg.exec();
    }
}

void AuxiliaryWidget::ClearHardwareSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "清空硬件默认参数", "是否删除硬件配置文件？\n清空前请确认备份！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
#ifndef WIN32
        system("rm /opt/luip/SP/Config/HardwareConfig.ls");
        system("sync");
#endif
        LuipMessageDialog msg("清除中\n", 1,this,MsgStyle::NONE);
        msg.exec();
    }
}

}
