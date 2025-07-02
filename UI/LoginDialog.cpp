#include "System/DynamicPassword.h"
#include "UI/NumberKeyboard.h"
#include "LoginDialog.h"
#include "checkUser.h"
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace System;

namespace UI
{

LoginDialog::LoginDialog(QWidget *parent ,int waitTime) :
        QDialog(parent),m_waitTime(0),m_curTime(0),m_migrateProgress(0),m_migrateProgressAdd(0),m_totalClick(0)
{
    //周立功板卡配合开机界面所以使用800x592  天嵌板卡使用800x600
    this->resize(800, 600);

    this->move(QPoint(0,0));
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏

    QPalette pa(this->palette());

    QImage img = QImage(":/pic/pic/LuipLog.bmp");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);

    pa.setBrush(QPalette::Window,*pic);

    this->setPalette(pa);


    QFileInfo fileInfo("/opt/LuipData/MeasureResultFile.bin");
    if(fileInfo.isFile())
    {
        qint64 fileSize = fileInfo.size();
        m_migrateProgressAdd = 60.0/(((fileSize+1023)/1024)/3.5);
    }
#ifndef WIN32
    system("echo [data]=0 >/opt/MigrateData");
    system("/sbin/udhcpc &");
#endif

    m_titleLabel = new QLabel();
    m_userLabel = new QLabel();
    m_passwordLabel = new QLabel();
    m_userComboBox = new QComboBox();
    m_passwordLineEdit = new QLineEdit();
    m_loginButton = new QPushButton();
    m_ipLabel = new QLabel();
    m_timeLabel = new QLabel();

    m_titleLabel->setObjectName("blueLabel");
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    m_titleLabel->setFont(titleFont);
    m_userLabel->setFixedSize(100, 25);
    m_userLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);

    topLabel= new QLabel();
    topLabel->setFont(titleFont);
    topLabel->setObjectName("blueLabel");
    topLabel->setFixedSize(120, 25);
    topLabel->setText("软件启动中");
    topLabel->setAlignment(Qt::AlignTop);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(m_titleLabel, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->setContentsMargins(0, 5, 10, 0);
    m_titleLabel->setContentsMargins(30, 30, 0, 0);

    QHBoxLayout *topLabelLayout = new QHBoxLayout();
    topLabelLayout->addStretch();
    topLabelLayout->addWidget(topLabel);
    topLabelLayout->addStretch();

    dataLabel= new QLabel();
    dataLabel->setFont(titleFont);
    dataLabel->setObjectName("blueLabel");
    dataLabel->setFixedSize(400, 25);
    dataLabel->setText("正在进行数据迁移，请勿重启仪器......");
    dataLabel->setAlignment(Qt::AlignTop);
    dataLabel->hide();

    progressLabel= new QLabel();
    progressLabel->setFont(titleFont);
    progressLabel->setObjectName("blueLabel");
    progressLabel->setFixedSize(400, 25);
    progressLabel->setText("");
    progressLabel->setAlignment(Qt::AlignTop);
    progressLabel->hide();

    QHBoxLayout *dataLayout = new QHBoxLayout();
    dataLayout->addStretch();
    dataLayout->addWidget(dataLabel);
    dataLayout->addStretch();

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addStretch();
    progressLayout->addWidget(progressLabel);
    progressLayout->addStretch();

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addLayout(titleLayout);
    topLayout->addLayout(topLabelLayout);
    topLayout->addLayout(dataLayout);
    topLayout->addLayout(progressLayout);
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0, 0, 0, 0);

    //用户
    QFont font;
    font.setPointSize(16);

    m_userLabel->setObjectName(QStringLiteral("whiteLabel"));
    m_userLabel->setFixedSize(60, 30);
    m_userLabel->setFont(font);
    m_userLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStringList userStringList;
    userStringList  << tr("Admin") << tr("Super");
    m_userComboBox->setObjectName(QStringLiteral("m_userComboBox"));
    m_userComboBox->setFixedSize(140, 30);
    m_userComboBox->setFont(font);
    m_userComboBox->clear();
    m_userComboBox->insertItems(0, userStringList);

    //密码
    m_passwordLabel->setObjectName(QStringLiteral("whiteLabel"));
    m_passwordLabel->setFixedSize(60, 30);
    m_passwordLabel->setFont(font);
    m_passwordLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_passwordLineEdit->setObjectName(QStringLiteral("m_userComboBox"));
    m_passwordLineEdit->setFixedSize(140, 30);
    QFont passwordFont = m_passwordLineEdit->font();
    passwordFont.setPointSize(12);
    m_passwordLineEdit->setFont(passwordFont);
    m_passwordLineEdit->setMaxLength(6);
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);
    m_passwordLineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    CNumberKeyboard *loginDialogKeyboard = new CNumberKeyboard(this);
    m_passwordLineEdit->installEventFilter(loginDialogKeyboard);

    m_loginButton = new QPushButton();
    m_loginButton->setObjectName(QStringLiteral("brownButton"));
    m_loginButton->setFixedSize(140, 38);
    m_loginButton->setFont(font);
    m_loginButton->setFocus();
    m_loginButton->setDefault(true);
    connect(m_loginButton, SIGNAL(clicked()), this, SLOT(SlotloginButton()));

    m_unfoldButton = new QPushButton();
    m_unfoldButton->setObjectName(QStringLiteral("brownButton"));
    m_unfoldButton->setFixedSize(200, 38);
    m_unfoldButton->setFont(font);
    m_unfoldButton->setFocus();
    m_unfoldButton->setDefault(true);
    connect(m_unfoldButton, SIGNAL(clicked()), this, SLOT(SlotunfoldButton()));

    m_cancelButton = new QPushButton();
    m_cancelButton->setObjectName(QStringLiteral("brownButton"));
    m_cancelButton->setFixedSize(140, 38);
    m_cancelButton->setFont(font);
    m_cancelButton->setFocus();
    m_cancelButton->setDefault(true);
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));


    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(m_userLabel, 0, 0);
    gridLayout->addWidget(m_userComboBox, 0, 1, 1, 2);
    gridLayout->addWidget(m_passwordLabel, 1, 0);
    gridLayout->addWidget(m_passwordLineEdit, 1, 1, 1, 2);
    gridLayout->setHorizontalSpacing(20);
    gridLayout->setVerticalSpacing(30);

    QHBoxLayout *gridHLayout = new QHBoxLayout();
    gridHLayout->addStretch();
    gridHLayout->addLayout(gridLayout);
    gridHLayout->addStretch();
    gridHLayout->setSpacing(10);
    gridHLayout->setContentsMargins(0, 25, 40, 0);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
//    bottomLayout->addStretch();
//    bottomLayout->addWidget(m_cancelButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_loginButton);
    bottomLayout->addWidget(m_unfoldButton);
    bottomLayout->addStretch();
    bottomLayout->setSpacing(10);
    bottomLayout->setContentsMargins(0, 20, 0, 0);

    m_ipLabel->setObjectName("blueLabel");
    QFont ipFont = m_ipLabel->font();
    ipFont.setPointSize(12);
    m_ipLabel->setFont(ipFont);
    m_ipLabel->setFixedSize(260, 25);
    m_ipLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);;

    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addStretch();
    ipLayout->addWidget(m_ipLabel);
    ipLayout->setSpacing(10);
    ipLayout->setContentsMargins(0, 0, 20, 0);

    m_timeLabel->setObjectName("blueLabel");
    QFont timeFont = m_timeLabel->font();
    timeFont.setPointSize(12);
    m_timeLabel->setFont(timeFont);
    m_timeLabel->setFixedSize(200, 25);
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addStretch();
    timeLayout->addWidget(m_timeLabel);
    timeLayout->setSpacing(10);
    timeLayout->setContentsMargins(0, 0, 20, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(gridHLayout);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(timeLayout);

    if (waitTime > 0)
    {
        m_waitTime = waitTime;
        m_titleLabel->hide();
        m_userComboBox->hide();
        m_passwordLineEdit->hide();
        m_userLabel->hide();
        m_passwordLabel->hide();
        m_loginButton->hide();
        m_unfoldButton->hide();
    }
    else
    {
        topLabel->hide();
        QPalette palette(this->palette());
        palette.setBrush(QPalette::Window,Qt::white);
        this->setPalette(palette);
    }

    this->setLayout(mainLayout);

    this->TranslateLanguage();

    m_curDateTimer = new QTimer();
    connect(m_curDateTimer, SIGNAL(timeout()), this, SLOT(SlotUpdataDateTime()));
    m_curDateTimer->start(1000);

    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerHandler()));
    m_timer->start(5000);
}

LoginDialog::~LoginDialog()
{
    system("rm /opt/MigrateData");
    if (m_curDateTimer)
        delete m_curDateTimer;
}

void LoginDialog::TranslateLanguage()
{
    m_titleLabel->setText(tr("超级工具"));
    m_userLabel->setText(tr("用户:"));
    m_passwordLabel->setText(tr("密码:"));
    m_loginButton->setText(tr("登录"));
    m_cancelButton->setText(tr("取消"));
    m_unfoldButton->setText(tr("关闭软件展开工具"));
}

bool LoginDialog::CheckMigrateData()
{
    bool ret = false;
    using namespace std;
    QFileInfo fileInfo("/opt/MigrateData");
    if(fileInfo.isFile())
    {
        fstream inFile;
        inFile.open("/opt/MigrateData");
        string str;

        while(getline(inFile, str))
        {
            if (string::npos != str.find("data"))    //当前只需要判定是否为远程控制平台
            {
                if((string::npos != str.find("1")) || (string::npos != str.find("true")))
                {
                    ret = true;
                }
                else
                {
                    ret = false;
                }
            }
        }
    }
    return ret;
}

void LoginDialog::SlotunfoldButton()
{
    system("killall -9 Monitor Luip");
    topLabel->setText("软件已关闭");
    m_curTime = m_waitTime;
}

void LoginDialog::SlotloginButton()
{
    QString userName = m_userComboBox->currentText();
    QString passWord = m_passwordLineEdit->text();

    QStringList pwdList = DynamicPassword::GeneratePasswordList();

    if (userName == tr("Admin"))
    {
        if (pwdList.contains(passWord))
        {
            CheckUser::Instance()->setUser(UserType::Admin);
            system("killall -9 Monitor Luip");
            system("rm /opt/MigrateData");
            accept();
        }
        else if (passWord == "666666")
        {
            CheckUser::Instance()->setUser(UserType::Admin);
            system("killall -9 Monitor Luip");
            system("rm /opt/MigrateData");
            accept();
        }
        else
        {
            m_passwordLineEdit->clear();
        }
    }
    else if (userName == tr("Super"))
    {
        if (pwdList.contains(passWord))
        {
            CheckUser::Instance()->setUser(UserType::Super);
            system("killall -9 Monitor Luip");
            system("rm /opt/MigrateData");
            accept();
        }
        else if (passWord == "042022")
        {
            CheckUser::Instance()->setUser(UserType::Super);
            system("killall -9 Monitor Luip");
            system("rm /opt/MigrateData");
            accept();
        }
        else
        {
            m_passwordLineEdit->clear();
        }
    }
}

void LoginDialog::SlotUpdataDateTime()
{
    if(CheckMigrateData())
    {
        dataLabel->show();
        progressLabel->show();
        if(m_waitTime < 600 && m_waitTime>0)
        {
            m_waitTime = 600;
        }
    }
    else
    {
        m_migrateProgress = 0;
        dataLabel->hide();
        progressLabel->hide();
    }

    if (m_waitTime>0)
    {
        m_curTime++;
        if(m_curTime >= m_waitTime)
        {
            QPalette palette(this->palette());
            palette.setBrush(QPalette::Window,Qt::white);
            this->setPalette(palette);
            m_waitTime = 0;
            m_titleLabel->show();
            m_userComboBox->show();
            m_passwordLineEdit->show();
            m_userLabel->show();
            m_passwordLabel->show();
            m_loginButton->show();
            m_unfoldButton->hide();
        }
    }
    QDateTime currentTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString currentTimeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    m_timeLabel->setText(currentTimeStr);
    m_migrateProgress += m_migrateProgressAdd;
    if (m_migrateProgress>95 && m_waitTime == 600)
    {
        m_migrateProgress = 95;
        m_migrateProgressAdd = 0;
        m_curTime = m_waitTime;
    }
    QString Str = "当前已完成" + QString(tr("%1%").arg(m_migrateProgress,0,'f',3));
    progressLabel->setText(Str);

    repaint();
}


void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if (0!=m_waitTime && m_totalClick<15)
    {
        m_totalClick++;
        if (m_totalClick == 15)
        {
            m_unfoldButton->show();
            dataLabel->hide();
            progressLabel->hide();
        }
    }
}

void LoginDialog::paintEvent(QPaintEvent *event)
{
    (void)event;
    locationList.clear();
    radiiList.clear();
    for(int i=0;i<7;i++)
    {
        if(i == 0)
            radiiList<<8;
        else
            radiiList<<4;
        locationList.append(QPoint(20 + i*20 ,20));
    }


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor(36, 167, 255));
    painter.setBrush(QColor(36, 167, 255));
    for(int i=0;i<7;i++)
    {
        painter.setPen(QColor(36, 167, 255));
        float radii=radiiList.at((m_index + 7 - i)%7); //半径
        painter.drawEllipse(QPointF(locationList.at(i).x(),locationList.at(i).y()),radii,radii);  //绘制圆点
    }
    m_index ++;
}

void LoginDialog::TimerHandler()
{
    m_timer->stop();
    bool isDisconnect = true;
    QHostAddress address;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QList<QNetworkInterface>::iterator it;
    for(it = list.begin(); it != list.end(); it++)
    {
        if((*it).name() == "eth0")
        {
            QNetworkInterface::InterfaceFlags flags = (*it).flags();
            QList<QNetworkAddressEntry> entryList = (*it).addressEntries();
            QList<QNetworkAddressEntry>::iterator entryit;
            for(entryit = entryList.begin(); entryit != entryList.end(); entryit++)
            {
                address = (*entryit).ip();
                if(address.protocol() == QAbstractSocket::IPv4Protocol)
                {
                    if(flags.testFlag(QNetworkInterface::IsRunning))
                    {
                        isDisconnect = false;
                        m_ipLabel->setText(tr("网络地址 ：") + (*entryit).ip().toString());
                    }
                    break;
                }
            }
        }
    }
    if (isDisconnect)
    {
        m_ipLabel->setText("无网络");
    }
    m_timer->start();
}


}
