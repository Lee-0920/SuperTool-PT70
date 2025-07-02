#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSysInfo>
#include <QStringListModel>
#include "System/Uncompress.h"
#include "System/AppCheck.h"
#include "Flow/FlowManager.h"
#include "UpdaterWidget.h"

#define UPDATERDIR "/mnt/udisk/Updater"

using namespace System;
using namespace Flow;

namespace UI
{
UpdaterWidget::UpdaterWidget(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setPointSize(13);

    m_kernelLabel = new QLabel();
    m_kernelLabel->setText("内核版本：");
    m_kernelLabel->setFixedSize(110, 30);
    m_kernelLabel->setFont(font);

    QString kVersion = QSysInfo::kernelVersion();
    kVersion.replace(QString("-EmbedSky"), QString(""));

    m_kVersionLabel = new QLabel();
    m_kVersionLabel->setText(QSysInfo::kernelType() + "-" + kVersion);
    m_kVersionLabel->setFixedSize(150, 30);
    m_kVersionLabel->setFont(font);

    m_fileSystemLabel = new QLabel();
    m_fileSystemLabel->setText("文件系统版本：");
    m_fileSystemLabel->setFixedSize(110, 30);
    m_fileSystemLabel->setFont(font);


    QString fsVersion;
    QFile file("/opt/version");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            fsVersion = in.readLine();
            file.close();
        }
    }
    else
    {
        fsVersion = "";
    }

    m_fsVersionLabel = new QLabel();
    m_fsVersionLabel->setText(fsVersion);
    m_fsVersionLabel->setFixedSize(150, 30);
    m_fsVersionLabel->setFont(font);

    m_findButton = new QPushButton();
    m_findButton->setObjectName("brownButton");
    m_findButton->setFixedSize(80,30);
    m_findButton->setText("查找");
    m_findButton->setFont(font);
    connect(m_findButton, SIGNAL(clicked(bool)),  this, SLOT(OnSearch()));

    QHBoxLayout *kVersionLayout = new QHBoxLayout();
    kVersionLayout->addWidget(m_kernelLabel);
    kVersionLayout->addWidget(m_kVersionLabel);
    kVersionLayout->addStretch();
    kVersionLayout->setSpacing(5);
    kVersionLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *fsVersionLayout = new QHBoxLayout();
    fsVersionLayout->addWidget(m_fileSystemLabel);
    fsVersionLayout->addWidget(m_fsVersionLabel);
    fsVersionLayout->addStretch();
    fsVersionLayout->setSpacing(5);
    fsVersionLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *versionLayout = new QVBoxLayout();
    versionLayout->addLayout(kVersionLayout);
    versionLayout->addLayout(fsVersionLayout);
    versionLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(m_findButton);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *upLayout = new QHBoxLayout();
    upLayout->addLayout(versionLayout);
    upLayout->addStretch();
    upLayout->addLayout(buttonLayout);
    upLayout->addStretch();
//    upLayout->setSpacing(50);
    upLayout->setContentsMargins(0, 0, 0, 0);

    m_fileTable = new FileTableWidget();
    m_fileTable->SetFixedSize(780, 427);

    QHBoxLayout *tableLayout = new QHBoxLayout();
    tableLayout->addStretch();
    tableLayout->addWidget(m_fileTable);
    tableLayout->addStretch();
    tableLayout->setSpacing(0);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *mainyout = new QVBoxLayout();
//    mainyout->addStretch();
    mainyout->addLayout(upLayout);
    mainyout->addLayout(tableLayout);
//    mainyout->addStretch();
    mainyout->setSpacing(0);
    mainyout->setContentsMargins(10, 15, 0, 0);

    this->setLayout(mainyout);

    m_msgDialog = new MessageDialog();
    m_msgDialog->setModal(false);
    m_msgDialog->hide();
}

UpdaterWidget::~UpdaterWidget()
{
    if (m_fileTable)
        delete m_fileTable;
}

void UpdaterWidget::OnSearch()
{
    QDir dir(UPDATERDIR);
    if(!dir.exists())
    {
        return;
    }

    m_nCOunt = 0;

    m_fileTable->setRowCount(0);


    QFileInfoList fileList = ExecuteSearch(dir);

    ShowFiles(fileList);
    m_nCOunt += fileList.size();
}

QFileInfoList UpdaterWidget::ExecuteSearch(QDir &dir)
{
//    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

//    for(int i = 0; i != folderList.size(); i++)
//    {
//         QString name = folderList.at(i).absoluteFilePath();
//         QFileInfoList childFileList = ExecuteSearch(name);
//         fileList.append(childFileList);
//    }

    dir.setFilter(QDir::Dirs| QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList fileList = dir.entryInfoList();


    return fileList;
}



void UpdaterWidget::ShowFiles(const QFileInfoList& files)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnUpdater(int)));
    QFont font;                           //字体
    font.setPointSize(12);

    int buttonCnt = 0;

    for(int i = 0 ; i < files.size();i++)
    {
        QFileInfo fileInfo = files.at(i);
        QString name = fileInfo.fileName();

        if (name != QString(".") && name != QString("..") && name != QString("luip"))
        {
            qint64 sizes = fileInfo.size();

            QTableWidgetItem* pNameItem = new QTableWidgetItem(name);
            QTableWidgetItem* pSizeItem = new QTableWidgetItem(tr("%1KB").arg(int(sizes+1023)/1024));

            int rowcount = m_fileTable->rowCount();
            m_fileTable->insertRow(rowcount);
            m_fileTable->setItem(rowcount,0,pNameItem);
            m_fileTable->setItem(rowcount,1,pSizeItem);

            QPushButton *button = new QPushButton();
            button->setObjectName("brownButton");
            button->setFixedSize(80,28);
            button->setFont(font);
            button->setText("升级");
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button, buttonCnt);
            m_fileTable->setCellWidget(rowcount,2,button);
            buttonCnt++;
        }
    }
}

void UpdaterWidget::OnUpdater(int index)
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "升级", "是否升级？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString filePath = QString(UPDATERDIR) + "/" + m_fileTable->item(index, 0)->text();
        FlowManager::Instance()->OnUpdater(filePath);
    }
}

}
