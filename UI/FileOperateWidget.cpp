#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringListModel>
#include "ImportDialog.h"
#include "FileOperateWidget.h"

namespace UI
{
FileOperateWidget::FileOperateWidget(QWidget *parent)
    : QWidget(parent),m_parent(parent)
{
    QFont dirFont;
    dirFont.setPointSize(13);

    m_dirLabel = new QLabel();
    m_dirLabel->setText("查找路径：");
    m_dirLabel->setFixedSize(80, 30);
    m_dirLabel->setFont(dirFont);

    m_dirComboBox = new QComboBox();
    m_dirComboBox->setObjectName("brownComboBox");
    QStringList dirList;

    dirList << tr("信息目录")
            << tr("数据目录")
            << tr("运行目录")
            << tr("程序目录");

    m_dirComboBox->setFont(dirFont);
    m_dirComboBox->insertItems(0,dirList);

    m_dirComboBox->setCurrentIndex(0);
    m_dirComboBox->setFixedSize(200,30);

    connect(m_dirComboBox, SIGNAL(activated(int)), this, SLOT(ComboBoxChanged(int)));

    m_findButton = new QPushButton();
    m_findButton->setObjectName("brownButton");
    m_findButton->setFixedSize(80,30);
    m_findButton->setText("查找");
    m_findButton->setFont(dirFont);
    connect(m_findButton, SIGNAL(clicked(bool)),  this, SLOT(SearchSlot()));


    m_deleteAllButton = new QPushButton();
    m_deleteAllButton->setObjectName("brownButton");
    m_deleteAllButton->setFixedSize(80,30);
    m_deleteAllButton->setText("删除全部");
    m_deleteAllButton->setFont(dirFont);
    connect(m_deleteAllButton, SIGNAL(clicked(bool)),  this, SLOT(DeleteAllSlot()));

    m_backupButton = new QPushButton();
    m_backupButton->setObjectName("brownButton");
    m_backupButton->setFixedSize(80,30);
    m_backupButton->setText("备份");
    m_backupButton->setFont(dirFont);
    connect(m_backupButton, SIGNAL(clicked(bool)),  this, SLOT(BackupSlot()));

    m_recoverButton = new QPushButton();
    m_recoverButton->setObjectName("brownButton");
    m_recoverButton->setFixedSize(80,30);
    m_recoverButton->setText("恢复");
    m_recoverButton->setFont(dirFont);
    connect(m_recoverButton, SIGNAL(clicked(bool)),  this, SLOT(RecoverSlot()));

    m_returnButton = new QPushButton();
    m_returnButton->setObjectName("brownButton");
    m_returnButton->setFixedSize(80,30);
    m_returnButton->setText("返回");
    m_returnButton->setFont(dirFont);
    connect(m_returnButton, SIGNAL(clicked(bool)),  this, SLOT(ReturnSlot()));

    m_importButton = new QPushButton();
    m_importButton->setObjectName("brownButton");
    m_importButton->setFixedSize(80,30);
    m_importButton->setText("导入");
    m_importButton->setFont(dirFont);
    m_importButton->setEnabled(false);
    connect(m_importButton, SIGNAL(clicked(bool)),  this, SLOT(ImportSlot()));

    QHBoxLayout *dirLayout = new QHBoxLayout();
    dirLayout->addWidget(m_dirLabel);
    dirLayout->addWidget(m_dirComboBox);
    dirLayout->addWidget(m_findButton);
    dirLayout->addWidget(m_returnButton);
    dirLayout->addWidget(m_importButton);
    dirLayout->addStretch();
    dirLayout->setSpacing(40);
    dirLayout->setContentsMargins(10, 0, 0, 0);

    QHBoxLayout *bLayout = new QHBoxLayout();
    bLayout->addStretch();
    bLayout->addWidget(m_backupButton);
    bLayout->addWidget(m_recoverButton);
    bLayout->addWidget(m_deleteAllButton);
    bLayout->addStretch();
    bLayout->setSpacing(40);
    bLayout->setContentsMargins(260, 0, 0, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(dirLayout);
    vLayout->addLayout(bLayout);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(0, 0, 0, 0);

    m_fileTable = new FileTableWidget();
    m_fileTable->SetFixedSize(780, 427);
    connect(m_fileTable,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(CurrentCellSlot(QTableWidgetItem*)));

    QHBoxLayout *tableLayout = new QHBoxLayout();
    tableLayout->addStretch();
    tableLayout->addWidget(m_fileTable);
    tableLayout->addStretch();
    tableLayout->setSpacing(0);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *mainyout = new QVBoxLayout();
//    mainyout->addStretch();
    mainyout->addLayout(vLayout);
    mainyout->addLayout(tableLayout);
//    mainyout->addStretch();
    mainyout->setSpacing(5);
    mainyout->setContentsMargins(0, 15, 0, 0);

    this->setLayout(mainyout);

    connect(this,SIGNAL(NextDirSignal(QString)),this,SLOT(NextDirSlot(QString)));
}

FileOperateWidget::~FileOperateWidget()
{
    if (m_fileTable)
        delete m_fileTable;
}

QString FileOperateWidget::GetPath()
{
    QString path = "";
    QString text = m_dirComboBox->currentText();

    if (text == "信息目录")
    {
        path = "/mnt/datadisk/Data";
    }
    else if (text == "数据目录")
    {
        path = "/opt/LuipData";
    }
    else if (text == "运行目录")
    {
        path = "/opt/luip";
    }
    else if (text == "程序目录")
    {
        path = "/opt";
    }

    return path;
}

void FileOperateWidget::LoopSearch(QString path)
{
    m_fileTable->ClearContent();

    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }

    m_nCOunt = 0;

    m_dirComboBox->setEnabled(false);
    m_findButton->setEnabled(false);
    m_importButton->setEnabled(false);
    m_fileTable->setEnabled(false);

    m_fileTable->setRowCount(0);

    QFileInfoList fileList = ExecuteSearch(dir);

    ShowFiles(fileList);

    m_nCOunt += fileList.size();

    m_dirComboBox->setEnabled(true);
    m_findButton->setEnabled(true);
    m_importButton->setEnabled(true);
    m_fileTable->setEnabled(true);
}

QFileInfoList FileOperateWidget::ExecuteSearch(QDir &dir)
{
    dir.setFilter(QDir::Dirs| QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList fileList = dir.entryInfoList();

    return fileList;
}

void FileOperateWidget::ShowFiles(const QFileInfoList& files)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(DeleteFileSlot(int)));
    QFont font;                           //字体
    font.setPointSize(12);

    int buttonCnt = 0;

    for(int i = 0 ; i < files.size();i++)
    {
        QFileInfo fileInfo = files.at(i);
        QString name = fileInfo.fileName();
        if (name != QString(".") && name != QString(".."))
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
            button->setText("删除");
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button, buttonCnt);
            m_fileTable->setCellWidget(rowcount,2,button);
            buttonCnt++;
        }
    }
}

void FileOperateWidget::ComboBoxChanged(int index)
{
    if (index == 2 || index == 3)
    {
        m_backupButton->setEnabled(false);
        m_recoverButton->setEnabled(false);
        m_deleteAllButton->setEnabled(false);
    }
    else
    {
        m_backupButton->setEnabled(true);
        m_recoverButton->setEnabled(true);
        m_deleteAllButton->setEnabled(true);
    }
}

void FileOperateWidget::SearchSlot()
{
    QString path = GetPath();
    LoopSearch(path);

    if (!m_dirList.empty())
    {
        m_dirList.clear();
    }

    m_dirList << path;
    m_returnButton->setEnabled(false);
    m_importButton->setEnabled(true);
}

void FileOperateWidget::DeleteFileSlot(int index)
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "删除文件", "是否删除文件/文件夹？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString filePath = GetPath() + "/" + m_fileTable->item(index, 0)->text();

        QDir dir(filePath);
        if(dir.exists())
        {
            #ifndef WIN32
                QString cmd = QString("rm -r ") + filePath;
                qDebug() <<  "cmd : " << cmd;
                system(cmd.toStdString().c_str());
            #endif
        }
        else if (QFile::exists(filePath))
        {
            #ifndef WIN32
                QString cmd = QString("rm ") + filePath;
                qDebug() <<  "cmd : " << cmd;
                system(cmd.toStdString().c_str());
            #endif
        }
        else
        {
            QMessageBox::warning(NULL, "删除文件", "文件/文件夹不存在！", QMessageBox::Yes);
        }

        SearchSlot();
    }
}

void FileOperateWidget::DeleteAllSlot()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "删除全部", "是否删除文件夹？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString dirPath = GetPath();

        QDir dir(dirPath);
        if(dir.exists())
        {
            #ifndef WIN32
                QString cmd = QString("rm -r ") + dirPath;
                qDebug() <<  "cmd : " << cmd;
                system(cmd.toStdString().c_str());
            #endif
        }
        else
        {
            QMessageBox::warning(NULL, "删除文件", "文件夹不存在！", QMessageBox::Yes);
        }

        SearchSlot();
    }
}

void FileOperateWidget::BackupSlot()
{
#ifndef WIN32

    QString filePath = GetPath();
    QStringList strlist = filePath.split(QRegExp("[/]"));

    QString backupPath = QString(SuperToolBakDir) + strlist.back();

    QDir backupDir(backupPath);
    if(backupDir.exists())
    {
        QString cmd = QString("rm -r ") + backupPath;
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }

    QDir fileDir(filePath);
    if(fileDir.exists())
    {
        QDir mbDir(SuperToolBakDir);
        if(!mbDir.exists())
        {
            QString cmd = QString("mkdir ") + SuperToolBakDir;
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }

        if (m_dirComboBox->currentIndex() == 1)
        {
            QString cmd = QString("mkdir ") + backupPath;
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());


            cmd = QString("cp ") + filePath + "/*.bin" + " "+ backupPath;
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }
        else
        {
            QString cmd = QString("cp -r ") + filePath + " " + backupPath;
            qDebug() <<  "cmd : " << cmd;
            system(cmd.toStdString().c_str());
        }
    }

    if(backupDir.exists())
    {
        QMessageBox::information(NULL, "备份", "备份成功！", QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(NULL, "备份", "备份失败！", QMessageBox::Yes);
    }
#endif
}

void FileOperateWidget::RecoverSlot()
{
#ifndef WIN32
    QString filePath = GetPath();
    QStringList strlist = filePath.split(QRegExp("[/]"));

    QString backupPath = QString(SuperToolBakDir) + strlist.back();

    QDir fileDir(filePath);
    if(fileDir.exists())
    {
        QString cmd = QString("rm -r ") + filePath;
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }


    QDir backupDir(backupPath);
    if(backupDir.exists())
    {
        QString cmd = QString("cp -r ") + backupPath + " " + filePath;
        qDebug() <<  "cmd : " << cmd;
        system(cmd.toStdString().c_str());
    }

    if(fileDir.exists())
    {
        SearchSlot();
        QMessageBox::information(NULL, "恢复", "恢复成功！", QMessageBox::Yes);
    }
    else
    {
        QMessageBox::warning(NULL, "恢复", "恢复失败！", QMessageBox::Yes);
    }
#endif
}

void FileOperateWidget::ReturnSlot()
{
    if (m_dirList.size() > 1)
    {
        m_dirList.pop_back();
        QString path = m_dirList.back();
        QString rootdir = GetPath();

        if (0 == QString::compare(path, rootdir, Qt::CaseSensitive))
        {
            m_returnButton->setEnabled(false);
        }

        QFileInfo fileInfo(path);
        if (fileInfo.isDir())
        {
            emit NextDirSignal(path);
        }
    }
}

void FileOperateWidget::ImportSlot()
{
    ImportDailog importDialog(m_dirList.back(), m_parent);
    if (importDialog.exec() == QDialog::Accepted)
    {
        LoopSearch(m_dirList.back());
    }
}

void FileOperateWidget::CurrentCellSlot(QTableWidgetItem* item)
{
    if (!m_dirList.empty())
    {
        if (item)
        {
            QString path = m_dirList.back() + "/" + item->text();

            QFileInfo fileInfo(path);
            if (fileInfo.isDir())
            {
                m_dirList << path;
                m_returnButton->setEnabled(true);
                emit NextDirSignal(path);
            }
        }
    }
}

void FileOperateWidget::NextDirSlot(QString path)
{
    LoopSearch(path);
}

}
