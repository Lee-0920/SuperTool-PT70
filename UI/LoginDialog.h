/*
 登陆界面 包括360、新浪、人人登录
 作者：╰☆奋斗ing❤孩子`
 博客地址：http://blog.sina.com.cn/liang19890820
 QQ：550755606
 Qt分享、交流群：26197884

 注：请尊重原作者劳动成果，仅供学习使用，请勿盗用，违者必究！
 */

#ifndef UI_FRAME_LOGINDIALOG_H
#define UI_FRAME_LOGINDIALOG_H

#include <QDialog>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QKeyEvent>
#include <QGridLayout>
#include <QTimer>
#include <QNetworkInterface>
#include <QList>
#include <QHostInfo>

namespace UI
{



class LoginDialog: public QDialog
{
Q_OBJECT

public:

    explicit LoginDialog(QWidget *parent = 0,int waitTime = 0);
    ~LoginDialog();

public:
    void TranslateLanguage();
    bool CheckMigrateData();

public slots:
    void SlotloginButton();
    void SlotunfoldButton();
    void SlotUpdataDateTime();
    void TimerHandler();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:

    QLabel *topLabel;
    QLabel *dataLabel;
    QLabel *progressLabel;
    QLabel *m_titleLabel;
    QLabel *m_userLabel;
    QLabel *m_passwordLabel;
    QComboBox *m_userComboBox;
    QLineEdit *m_passwordLineEdit;
    QPushButton *m_loginButton;
    QPushButton *m_unfoldButton;
    QPushButton *m_cancelButton;
    QTimer *m_curDateTimer;
    QTimer *m_timer;
    QLabel *m_ipLabel;
    QLabel *m_timeLabel;
    int m_waitTime;
    int m_curTime;
    float m_migrateProgress;
    float m_migrateProgressAdd;
    char m_totalClick;

    QList<float> radiiList;     //直径列表
    QList<QPoint> locationList; //圆点坐标列表
    int m_index;
};

}

#endif //UI_FRAME_LOGINDIALOG_H
