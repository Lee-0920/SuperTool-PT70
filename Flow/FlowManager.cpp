#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSysInfo>
#include <QFileInfo>
#include <QProcess>
#include <QApplication>
#include <QDesktopWidget>
#include <QStringListModel>
#include "System/Uncompress.h"
#include "System/AppCheck.h"
#include "UI/MessageDialog.h"
#include "FlowManager.h"


using namespace System;
using namespace UI;

namespace Flow
{

std::unique_ptr<FlowManager> FlowManager::m_instance(nullptr);

/**
 * @brief 获取全局唯一的 FlowManager 对象（单件）。
 */
FlowManager* FlowManager::Instance()
{
    if (!m_instance)
    {
        m_instance.reset(new FlowManager);
    }

    return m_instance.get();
}

FlowManager::FlowManager()
    :m_isFlowRunnig(false)
{
    m_flowThread = new QThread();
    this->moveToThread(m_flowThread);
    m_flowThread->start();

    connect(this, SIGNAL(UpdaterSignal(QString)), this, SLOT(UpdaterSlot(QString)));
    connect(this, SIGNAL(StartLuipSignal()), this, SLOT(StartLuipSlot()));
}

FlowManager::~FlowManager()
{

}

void FlowManager::OnUpdater(QString path)
{
    if (m_isFlowRunnig)
    {
        return;
    }

    m_isFlowRunnig = true;
    emit UpdaterSignal(path);
}

void FlowManager::UpdaterSlot(QString path)
{
     MessageDialog msgDialog;
     msgDialog.SetText("升级中，请稍等 ... ");
     msgDialog.move ((QApplication::desktop()->width() - msgDialog.width())/2,(QApplication::desktop()->height() - msgDialog.height())/2);
     msgDialog.show();

    try
    {
        qDebug() << "filePath = " << path;

        int waitTime = 120;

        QString UpdaterShPath = "";
        QStringList stringList;

        if (path == "/mnt/udisk/Updater/luip")
        {
            UpdaterShPath = "/mnt/udisk/Updater/luip/Updater.sh";
            stringList << UpdaterShPath;
            stringList << path;

            waitTime = 60;
        }
        else
        {
            QStringList strlist = path.split(QRegExp("[.]"));
            if (strlist.back() == "upg")
            {
                //****************** 解压 **************************
                QFileInfo original(path);
                QString destName = original.completeBaseName();
                QString destDirectory = QString("/opt/Unzip") + "/" + destName + "/";

                qDebug() << "destDirectory = " << destDirectory;

                QStringList listPic;
                if (false == Uncompress::UnZipToDirectory(path.toStdString(), destDirectory.toStdString(), listPic, "LuipAdmin@2017"))
                {
                    msgDialog.hide();
                    QMessageBox::warning(NULL, "升级", "解压失败！", QMessageBox::Yes);
                    return;
                }

                QStringList result = listPic.filter("Updater.sh");

                if (result.size() > 0)
                {
                    UpdaterShPath = result.at(0);
                }
                stringList << UpdaterShPath;
                stringList << destDirectory;
            }
        }

        //****************** 升级 **************************
        QFile file(UpdaterShPath);
        if (file.exists())
        {
            QProcess process;
            if(process.startDetached("sh", stringList))
            {
                sleep(waitTime);

                bool ret = false;

                for (int i = 0; i < 3; i++)
                {
                    if ('0' != AppCheck("ps -ef|grep Luip|grep -v grep|wc -l"))
                    {
                        msgDialog.hide();

                        system("killall -9 Luip");

                        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "升级", "升级成功，是否关闭应用程序？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        if(rb == QMessageBox::Yes)
                        {
                            system("killall -9 Luip");
                            emit UpdaterWindowSignal();
                        }

                        ret = true;
                        break;
                    }

                    sleep(10);
                }

                if (!ret)
                {
                    msgDialog.hide();
                    QMessageBox::warning(NULL, "升级", "升级失败！", QMessageBox::Yes);
                }
            }
            else
            {
                msgDialog.hide();
                QMessageBox::warning(NULL, "升级", "升级脚本无法启动！", QMessageBox::Yes);
            }
        }
        else
        {
            msgDialog.hide();
            QMessageBox::warning(NULL, "升级", "升级脚本不存在！", QMessageBox::Yes);
        }

        msgDialog.hide();
    }
    catch(std::exception e)
    {
        m_isFlowRunnig = false;
        msgDialog.hide();
        qDebug() << "std exception ";
    }

    m_isFlowRunnig = false;
}

void FlowManager::OnStartLuip()
{
    if (m_isFlowRunnig)
    {
        return;
    }

    m_isFlowRunnig = true;
    emit StartLuipSignal();
}

void FlowManager::StartLuipSlot()
{
    MessageDialog msgDialog;
    msgDialog.SetText("启动中，请稍等 ... ");
    msgDialog.move ((QApplication::desktop()->width() - msgDialog.width())/2,(QApplication::desktop()->height() - msgDialog.height())/2);
    msgDialog.show();

    try
    {
        QString path = "/opt/luip/Luip";
        if (QFile::exists(path))
        {
            if ('0' != AppCheck("ps -ef|grep Luip|grep -v grep|wc -l"))
            {
                system("killall -9 Luip");
            }

            QThread::sleep(5);

            QProcess::startDetached(path);

            QThread::sleep(5);

            if(QProcess::startDetached(path))
            {
                sleep(60);

                bool ret = false;

                for (int i = 0; i < 3; i++)
                {
                    if ('0' != AppCheck("ps -ef|grep Luip|grep -v grep|wc -l"))
                    {
                        msgDialog.hide();
                        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "", "应用程序启动成功，是否关闭应用程序？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                        if(rb == QMessageBox::Yes)
                        {
                            system("killall -9 Luip");
                            emit UpdaterWindowSignal();
                        }

                        ret = true;
                        break;
                    }

                    sleep(10);
                }

                if (!ret)
                {
                    msgDialog.hide();
                    QMessageBox::warning(NULL, "重启应用程序", "启动应用程序失败！", QMessageBox::Yes);
                }
            }
            else
            {
                msgDialog.hide();
                QMessageBox::warning(NULL, "重启应用程序", "无法启动应用程序！", QMessageBox::Yes);
            }
        }
        else
        {
            msgDialog.hide();
            QMessageBox::warning(NULL, "重启应用程序", "应用程序不存在！", QMessageBox::Yes);
        }
    }
    catch(std::exception e)
    {
        m_isFlowRunnig = false;
        msgDialog.hide();
        qDebug() << "std exception ";
    }

    m_isFlowRunnig = false;
}

}
