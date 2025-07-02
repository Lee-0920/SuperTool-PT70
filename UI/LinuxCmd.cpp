#include "LinuxCmd.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "InputKeyBoard.h"
#include <QProcess>
#include "checkUser.h"

LinuxCmd::LinuxCmd(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setPointSize(13);

    m_ReadWindow = new QTextBrowser(this);
    m_ReadWindow->setFixedSize(600,250);
    m_ReadWindow->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 36px;}");

    m_WriteWindow = new QTextEdit(this);
    m_WriteWindow->setFixedSize(250,100);
    m_WriteWindow->setLineWidth(100);

    QStringList BoxList;
    BoxList<<"后50行"<<"后100行"<<"后150行"<<"后200行";
    m_logGetLine = new QComboBox();
    m_logGetLine->setFixedSize(120,40);
    m_logGetLine->addItems(BoxList);
    m_logGetLine->setFont(font);

    QPushButton *sendCmdBnt = new QPushButton(this);
    sendCmdBnt->setFixedSize(100,50);
    sendCmdBnt->setObjectName(QStringLiteral("brownButton"));
    sendCmdBnt->setText("发送");
    connect(sendCmdBnt,SIGNAL(clicked()),this,SLOT(SlotsSend()));

    QPushButton *openkeyBnt = new QPushButton(this);
    openkeyBnt->setFixedSize(100,50);
    openkeyBnt->setObjectName(QStringLiteral("brownButton"));
    openkeyBnt->setText("打开键盘");
    connect(openkeyBnt,SIGNAL(clicked()),this,SLOT(SlotsOpenKey()));

    QPushButton *openLog = new QPushButton(this);
    openLog->setFixedSize(100,50);
    openLog->setObjectName(QStringLiteral("warnButton"));
    openLog->setText("查看日志");
    connect(openLog,SIGNAL(clicked()),this,SLOT(SlotsOpenLog()));

    QHBoxLayout *bntLayout = new QHBoxLayout();
    bntLayout->addWidget(sendCmdBnt);
    bntLayout->addWidget(openkeyBnt);
    bntLayout->addSpacing(50);
    bntLayout->addWidget(openLog);
    bntLayout->addSpacing(20);
    bntLayout->addWidget(m_logGetLine);
    bntLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_ReadWindow);
    mainLayout->addWidget(m_WriteWindow);
    mainLayout->addLayout(bntLayout);

    if (CheckUser::Instance()->getUser() != UserType::Super)
    {
        sendCmdBnt->setEnabled(false);
        openkeyBnt->setEnabled(false);
    }

    this->setLayout(mainLayout);
}

void LinuxCmd::SlotsSend()
{
//#ifndef WIN32
    QProcess process;
    QString result = "";
    process.start(m_WriteWindow->toPlainText());
    if(process.waitForFinished())
    {
        result = process.readAllStandardOutput();
    }
    else
    {
        result = "超时";
    }

    m_ReadWindow->append(result);
//#endif
}

void LinuxCmd::SlotsOpenLog()
{
#ifndef WIN32
    QProcess process;
    QString readLog = "/opt/LuipData/getLog.sh";
    QString cmd;
    switch (m_logGetLine->currentIndex()) {
    case 0:
        cmd = "echo 'cat /opt/LuipData/log.txt | tail -n 50'>/opt/LuipData/getLog.sh";
        break;
    case 1:
        cmd = "echo 'cat /opt/LuipData/log.txt | tail -n 100'>/opt/LuipData/getLog.sh";
        break;
    case 2:
        cmd = "echo 'cat /opt/LuipData/log.txt | tail -n 150'>/opt/LuipData/getLog.sh";
        break;
    case 3:
        cmd = "echo 'cat /opt/LuipData/log.txt | tail -n 200'>/opt/LuipData/getLog.sh";
        break;
    default:
        break;
    }

    system(cmd.toStdString().c_str());
    QString result = "";
    process.start(readLog);
    process.waitForStarted();
    if(process.waitForFinished())
    {
        result = process.readAllStandardOutput();
    }
    else
    {
        result = "超时";
    }
    m_ReadWindow->clear();
    m_ReadWindow->append(result);
#endif
}

void LinuxCmd::SlotsOpenKey()
{
    InputKeyBoard *numKbd = InputKeyBoard::Instance();

    if(true == numKbd->isHidden())
    {
        numKbd->myMove(270, 340, 0, 0);
        numKbd->show();
    }
}
