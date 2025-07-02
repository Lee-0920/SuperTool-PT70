#include "LuipMessageDialog.h"
#include <QDebug>
#include <QProcess>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>

namespace UI
{

LuipMessageDialog::LuipMessageDialog(QWidget *parent):QDialog(parent),deadTimer(nullptr)
{
    Init("", MsgStyle::ONLYOK);
}

LuipMessageDialog::LuipMessageDialog(const QString &text,
                             QWidget *parent,
                             MsgStyle msgstyle,
                             bool isWarning)
    :QDialog(parent),deadTimer(nullptr)
{
    Init(text, msgstyle, isWarning);
}

LuipMessageDialog::LuipMessageDialog(const QString &text,
                             int deadTime,
                             QWidget *parent,
                             MsgStyle msgstyle,
                             bool isWarning) :QDialog(parent),deadTimer(nullptr)
{
    m_deadTime = deadTime;
    if(m_deadTime > 0)
    {
        Init(text, msgstyle, isWarning, true);
    }
    else
    {
        Init(text, msgstyle, isWarning, false);
    }
}

void LuipMessageDialog::Init(const QString &text,
                         MsgStyle msgstyle,
                         bool isWarning,
                         bool withDeadTime)
{
#ifdef _CS_X86_WINDOWS
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
#endif

   // this->resize(280, 205);
    this->setMinimumSize(280,205);

    m_logoLabel = new QLabel(this);

    m_logoLabel->setFixedSize(36, 36);
    m_logoLabel->setPixmap(QPixmap(":/pic/pic/WQIMC.ico"));
    m_logoLabel->setScaledContents(true);

    QHBoxLayout *logoLayout = new QHBoxLayout();
    logoLayout->addStretch();
    logoLayout->addWidget(m_logoLabel,1,Qt::AlignRight|Qt::AlignVCenter);
    logoLayout->setSpacing(0);

    QVBoxLayout *topLayout = new QVBoxLayout();
    topLayout->addLayout(logoLayout);
    topLayout->addStretch();
    topLayout->setSpacing(0);

    QFont font;
    font.setPointSize(14);

    m_textLabel = new QLabel(this);
    m_textLabel->setObjectName(QStringLiteral("whiteLabel"));
    m_textLabel->setFont(font);
    m_textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_textLabel->setText(text);
    m_textLabel->setWordWrap(true);
    m_textLabel->setMinimumSize(240, 80);
    m_textLabel->adjustSize();

    QHBoxLayout *textLayout = new QHBoxLayout();
    textLayout->addStretch();
    textLayout->addWidget(m_textLabel);
    textLayout->addStretch();

    if(withDeadTime == true)
    {
        m_deadLabel = new QLabel(this);
        m_deadLabel->setObjectName(QStringLiteral("whiteLabel"));
        m_deadLabel->setFont(font);
        m_deadLabel->setFixedSize(60, 20);
        m_deadLabel->setAlignment(Qt::AlignCenter);
        m_deadLabel->setText(QString::number(m_deadTime) + " 秒");
    }

    m_confirmButton = new QPushButton(this);
    m_confirmButton->setObjectName(QStringLiteral("brownButton"));
    m_confirmButton->setFixedSize(60, 30);
    m_confirmButton->setFont(font);
    m_confirmButton->setFocus();
    m_confirmButton->setDefault(true);
    m_confirmButton->setText(tr("确认"));
    connect(m_confirmButton, SIGNAL(clicked()), this, SLOT(SlotConfirmButton()));

    m_cancelButton = new QPushButton(this);
    m_cancelButton->setObjectName(QStringLiteral("brownButton"));
    m_cancelButton->setFixedSize(60, 30);
    m_cancelButton->setFont(font);
    m_cancelButton->setFocus();
    m_cancelButton->setDefault(true);
    m_cancelButton->setText(tr("取消"));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(SlotCancelButton()));

    QHBoxLayout *bottomBtnLayout = new QHBoxLayout();
    switch(msgstyle)
    {
    case MsgStyle::ONLYOK:
        bottomBtnLayout->addWidget(m_confirmButton);
        m_cancelButton->hide();
        break;
    case MsgStyle::OKANDCANCEL:
        bottomBtnLayout->addWidget(m_confirmButton);
        bottomBtnLayout->addWidget(m_cancelButton);
        break;
    case MsgStyle::ONLYCANCEL:
        m_confirmButton->hide();
        bottomBtnLayout->addWidget(m_cancelButton);
        break;
    case MsgStyle::NONE:
        m_cancelButton->hide();
        m_confirmButton->hide();
        bottomBtnLayout->addWidget(m_cancelButton);
        break;
    }
    bottomBtnLayout->setSpacing(10);

    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->addStretch(1);
    bottomLayout->addLayout(textLayout);
    if(withDeadTime == true)
    {
        bottomLayout->addWidget(m_deadLabel, 0, Qt::AlignHCenter);//1
    }
    bottomLayout->addStretch(1);//2
    bottomLayout->addLayout(bottomBtnLayout,1);
    bottomLayout->addStretch(1);
    bottomLayout->setSpacing(5);//10

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout,1);
    mainLayout->addLayout(bottomLayout,3);
    mainLayout->setSpacing(10);

    if(withDeadTime == true)
    {
        deadTimer = new QTimer(this);
        deadTimer->setInterval(1000);

        connect(this->deadTimer, SIGNAL(timeout()), this, SLOT(SlotDeadTimeCount()));

        connect(this, SIGNAL(SignalDeadTimeReboot()), this, SLOT(SlotDeadTimeReboot()));

        deadTimer->start();
    }

    this->setLayout(mainLayout);
}

LuipMessageDialog::~LuipMessageDialog()
{

}

void LuipMessageDialog::SlotConfirmButton()
{
    if(deadTimer != nullptr)
    {
        deadTimer->stop();
    }
    this->accept();
}

void LuipMessageDialog::SlotCancelButton()
{
    if(deadTimer != nullptr)
    {
        deadTimer->stop();
    }
    this->reject();
}

void LuipMessageDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    int height = 50;
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPixmap(
            QRect(1, 1, this->width() - 2 * 1,
                  this->height() - 2 * 1), QPixmap(":/pic/pic/17_big")); //DEFAULT_SKIN
    painter.drawRect(
            QRect(1, height, this->width() - 2 * 1,
                  this->height() - height - 1));
}


void LuipMessageDialog::SlotDeadTimeCount()
{
    m_deadTime--;
    this->m_deadLabel->setText(QString::number(m_deadTime) + " 秒");
    if(m_deadTime <= 0)
    {
        if(deadTimer != nullptr)
        {
            deadTimer->stop();
        }
        emit SignalDeadTimeReboot();
    }
}

void LuipMessageDialog::SlotDeadTimeReboot()
{
//    #ifdef _CS_ARM_LINUX
//        QProcess::execute("reboot");
//    #endif
    this->accept();
}

void LuipMessageDialog::SetText(const QString &text)
{
    m_textLabel->setText(text);
}
}
