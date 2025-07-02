/*
 */

#ifndef UI_LUIPMESSAGEBOXDIALOG_H
#define UI_LUIPMESSAGEBOXDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

namespace UI
{

enum MsgStyle
{
    ONLYOK,OKANDCANCEL,ONLYCANCEL,NONE
};

class LuipMessageDialog: public QDialog
{
Q_OBJECT

public:
    explicit LuipMessageDialog(QWidget *parent = 0);
    explicit LuipMessageDialog(const QString &text,
                           QWidget *parent = 0,
                           MsgStyle msgstyle = MsgStyle::ONLYOK,
                           bool isWarning = false);
    explicit LuipMessageDialog(const QString &text,
                           int deadTime,
                           QWidget *parent = 0,
                           MsgStyle msgstyle = MsgStyle::ONLYCANCEL,
                           bool isWarning = false);

    ~LuipMessageDialog();
    void SetText(const QString &text);

signals:
    void SignalDeadTimeReboot();

private slots:
    void SlotConfirmButton();
    void SlotCancelButton();
    void SlotDeadTimeReboot();
    void SlotDeadTimeCount();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLabel *m_logoLabel;
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;
    QLabel *m_textLabel;

    QTimer *deadTimer;
    int m_deadTime;
    QLabel *m_deadLabel;
    void Init(const QString &text,
              MsgStyle msgstyle,
              bool isWarning = false,
              bool withDeadTime = false);
};

}

#endif //UI_LUIPMESSAGEBOXDIALOG_H
