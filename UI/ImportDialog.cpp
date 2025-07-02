#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringListModel>
#include "ImportDialog.h"

#define ImportDir  ("/mnt/udisk")

namespace UI
{
ImportDailog::ImportDailog(QString destDir, QWidget *parent)
    : QDialog(parent),
      m_destDir(destDir)
{
//    this->adjustSize();
    this->resize(800, 600);

    QFont dirFont;
    dirFont.setPointSize(13);

    m_returnButton = new QPushButton();
    m_returnButton->setObjectName("brownButton");
    m_returnButton->setFixedSize(80,30);
    m_returnButton->setText("上一级");
    m_returnButton->setFont(dirFont);
    connect(m_returnButton, SIGNAL(clicked(bool)),  this, SLOT(ReturnSlot()));

    m_quitButton = new QPushButton();
    m_quitButton->setObjectName("brownButton");
    m_quitButton->setFixedSize(80,30);
    m_quitButton->setText("退出");
    m_quitButton->setFont(dirFont);
    connect(m_quitButton, SIGNAL(clicked(bool)),  this, SLOT(close()));

    QHBoxLayout *bLayout = new QHBoxLayout();
    bLayout->addWidget(m_returnButton);
    bLayout->addStretch();
    bLayout->addWidget(m_quitButton);
    bLayout->setSpacing(40);
    bLayout->setContentsMargins(10, 0, 10, 0);

    m_fileTable = new FileTableWidget();
    m_fileTable->SetFixedSize(780, 520);
    connect(m_fileTable,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(CurrentCellSlot(QTableWidgetItem*)));

    QHBoxLayout *tableLayout = new QHBoxLayout();
    tableLayout->addStretch();
    tableLayout->addWidget(m_fileTable);
    tableLayout->addStretch();
    tableLayout->setSpacing(0);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *mainyout = new QVBoxLayout();
//    mainyout->addStretch();
    mainyout->addLayout(bLayout);
    mainyout->addLayout(tableLayout);
   mainyout->addStretch();
    mainyout->setSpacing(20);
    mainyout->setContentsMargins(0, 20, 0, 20);

    this->setLayout(mainyout);

    connect(this,SIGNAL(NextDirSignal(QString)),this,SLOT(NextDirSlot(QString)));

    qDebug() << "m_destDir = " << m_destDir;

    Search();
}

ImportDailog::~ImportDailog()
{
    if (m_fileTable)
        delete m_fileTable;
}

void ImportDailog::LoopSearch(QString path)
{
    m_fileTable->ClearContent();
    QDir dir(path);
    if(!dir.exists())
    {
        return;
    }

    m_fileTable->setRowCount(0);

    QFileInfoList fileList = ExecuteSearch(dir);

    ShowFiles(fileList);
}

QFileInfoList ImportDailog::ExecuteSearch(QDir &dir)
{
    dir.setFilter(QDir::Dirs| QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList fileList = dir.entryInfoList();


    return fileList;
}

void ImportDailog::ShowFiles(const QFileInfoList& files)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(ImportFileSlot(int)));
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
            button->setText("导入");
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button, buttonCnt);
            m_fileTable->setCellWidget(rowcount,2,button);
            buttonCnt++;
        }
    }
}

void ImportDailog::Search()
{
    LoopSearch(ImportDir);

    if (!m_dirList.empty())
    {
        m_dirList.clear();
    }

    m_dirList << ImportDir;
    m_returnButton->setEnabled(false);
}

void ImportDailog::ImportFileSlot(int index)
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "", "是否导入文件/文件夹？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QString filePath = QString(ImportDir) + "/" + m_fileTable->item(index, 0)->text();

        QDir dir(m_destDir);
        if(dir.exists())
        {
            if (QFile::exists(filePath))
            {
                QString cmd = QString("cp -r ") + filePath + " " + m_destDir + "/";
                qDebug() <<  "cmd : " << cmd;
                system(cmd.toStdString().c_str());
                accept();
            }
            else
            {
                QMessageBox::warning(NULL, "", "导入文件/文件夹不存在！", QMessageBox::Yes);
            }
        }
        else
        {
            QMessageBox::warning(NULL, "", "目标路径不存在！", QMessageBox::Yes);
        }
    }
}

void ImportDailog::ReturnSlot()
{
    if (m_dirList.size() > 1)
    {
        m_dirList.pop_back();
        QString path = m_dirList.back();

        if (0 == QString::compare(path, ImportDir, Qt::CaseSensitive))
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

void ImportDailog::CurrentCellSlot(QTableWidgetItem* item)
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

void ImportDailog::NextDirSlot(QString path)
{
    LoopSearch(path);
}

}
