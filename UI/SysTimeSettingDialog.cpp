#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <time.h>
#include <stdio.h>
#ifndef  _CS_X86_WINDOWS
#include <sys/time.h>
#endif
#include <QApplication>
#include "SysTimeSettingDialog.h"

namespace UI
{

SysTimeSettingDialog::SysTimeSettingDialog(QWidget *parent) : QDialog(parent)
{
    this->resize(300, 200);
//    this->setWindowFlags(Qt::WindowSystemMenuHint);
    this->setWindowTitle("系统时间设置");
    this->setStyleSheet("background-color: rgb(240, 250, 255);");
    move ((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    QFont font;                           //字体
    font.setPointSize(12);                //字体大小

    QLabel *dayLabel = new QLabel();
    dayLabel->setText("日期");
    dayLabel->setFixedSize(40, 30);
    dayLabel->setFont(font);

    m_dayEdit = new QMyEdit();
    m_dayEdit->setFixedSize(150, 30);
    m_dayEdit->setFont(font);

    QHBoxLayout *dayLayout = new QHBoxLayout();
    dayLayout->addStretch();
    dayLayout->addWidget(dayLabel);
    dayLayout->addWidget(m_dayEdit);
    dayLayout->addStretch();

    QLabel *timeLabel = new QLabel();
    timeLabel->setText("时间");
    timeLabel->setFixedSize(40, 30);
    timeLabel->setFont(font);

    m_timeEdit = new QLineEdit();
    m_timeEdit->setFixedSize(150, 30);
    m_timeEdit->setFont(font);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addStretch();
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(m_timeEdit);
    timeLayout->addStretch();

    m_okButton = new QPushButton();
    m_okButton->setObjectName("");
    m_okButton->setText("确认");
    m_okButton->setFont(font);
    m_okButton->setFixedSize(90, 35);

    m_noButton = new QPushButton();
    m_noButton->setObjectName("");
    m_noButton->setText("取消");
    m_noButton->setFont(font);
    m_noButton->setFixedSize(90, 35);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_noButton);
    buttonLayout->addStretch();
    buttonLayout->setSpacing(40);
    buttonLayout->setContentsMargins(0, 20, 0, 0);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addStretch();
    vLayout->addLayout(dayLayout);
    vLayout->addLayout(timeLayout);
    vLayout->addLayout(buttonLayout);
    vLayout->addStretch();
    vLayout->setSpacing(20);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(vLayout);
    mainLayout->addStretch();

    this->setLayout(mainLayout);

    m_dayCaledar = new QCalendarWidget();
    m_dayCaledar->hide();

    m_timeEdit->installEventFilter(CNumberKeyboard::Instance());

    connect(m_dayEdit, SIGNAL(LineEditClicked()), this, SLOT(ChoseMinDay()));

    connect(m_dayCaledar, SIGNAL(clicked(const QDate &)), this, SLOT(ChangeMinDay()));

    connect(m_okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(m_noButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

SysTimeSettingDialog::~SysTimeSettingDialog()
{
    if (m_dayEdit)
        delete m_dayEdit;
}

void SysTimeSettingDialog::SetTime()
{
    QString dayValue = m_dayEdit->text();
    QStringList dayList = dayValue.split("-");

    if(!dayValue.isEmpty())
    {
        if(dayList.at(0).toInt() < 1970 || dayList.at(0).toInt()>= 2038)
        {
            QMessageBox msg1(QMessageBox::Icon::Warning, "", "输入年份超范围！", QMessageBox::Yes);
            if(msg1.exec()== QDialog::Accepted)
            {
                return;
            }
        }
    }

    QString timeValue = m_timeEdit->text();
    QStringList timeList = timeValue.split(":");

    QString tmpPattern("^\\d{1,2}$");  //匹配1-2位整数
    QRegExp tmpRx(tmpPattern);

    if(!timeValue.isEmpty())
    {
        if(timeList.size() != 3 ||
           !tmpRx.exactMatch(timeList.at(0)) || timeList.at(0).toInt() < 0 || timeList.at(0).toInt() > 23 ||
           !tmpRx.exactMatch(timeList.at(1)) || timeList.at(1).toInt() < 0 || timeList.at(1).toInt() > 59 ||
           !tmpRx.exactMatch(timeList.at(2)) || timeList.at(2).toInt() < 0 || timeList.at(2).toInt() > 59 )
        {
            QMessageBox msg2(QMessageBox::Icon::Warning, "", "输入时间格式不正确！", QMessageBox::Yes);
            if(msg2.exec()== QDialog::Accepted)
            {
                return;
            }
        }
    }

#ifdef _CS_ARM_LINUX
    struct tm *t_tm;
    struct timeval t_timeval;
    time_t t_timet;

    t_timet=time(NULL);
    t_tm=localtime(&t_timet);
    if(timeList.size() == 3)
    {
        t_tm->tm_hour = timeList.at(0).toInt();
        t_tm->tm_min = timeList.at(1).toInt();
        t_tm->tm_sec = timeList.at(2).toInt();
    }

    if(dayList.size() == 3)
    {
        t_tm->tm_year = dayList.at(0).toInt() - 1900;
        t_tm->tm_mon = dayList.at(1).toInt() - 1;
        t_tm->tm_mday = dayList.at(2).toInt();
    }

    t_timet=mktime(t_tm);

    t_timeval.tv_sec = t_timet;
    t_timeval.tv_usec = 0;

    int rec = settimeofday(&t_timeval,NULL);
    QString year = QString("%1").arg(t_tm->tm_year);
    QString month = QString("%1").arg(t_tm->tm_mon);
    QString day = QString("%1").arg(t_tm->tm_mday);
    QString hour = QString("%1").arg(t_tm->tm_hour);
    QString minute = QString("%1").arg(t_tm->tm_min);
    QString second = QString("%1").arg(t_tm->tm_sec);
    QString str= "date -s " + year + month + day + hour + minute + "." + second;

    //强制写入到CMOS
    system("hwclock -w");

    return;
#endif

}

void SysTimeSettingDialog::ChoseMinDay()
{
    m_dayCaledar->setFixedSize(300, 300);
    m_dayCaledar->move(130, 120);
    m_dayCaledar->setLocale(QLocale(QLocale::Chinese, QLocale::China));

    m_dayCaledar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    if (m_dayCaledar->isHidden())
    {
        m_dayCaledar->show();
    }
    else
    {
        m_dayCaledar->hide();
    }
}

void SysTimeSettingDialog::ChangeMinDay()
{
    QDate date = m_dayCaledar->selectedDate();
    QString dateString;
    QString monthsign;
    QString daysign;
    if (date.month() >= 10)
    {
        monthsign = "-";
    }
    else
    {
        monthsign = "-0";
    }

    if (date.day() >= 10)
    {
        daysign = "-";
    }
    else
    {
        daysign = "-0";
    }

    dateString = QString::number(date.year())+monthsign+QString::number(date.month())+daysign+QString::number(date.day());

    m_dayEdit->setText(dateString);
    m_dayCaledar->hide();
}

void SysTimeSettingDialog::showEvent(QShowEvent *event)
{
    (void)event;
    this->setFocus();
    if (!m_dayCaledar->isHidden())
    {
        m_dayCaledar->hide();
    }
}

}
