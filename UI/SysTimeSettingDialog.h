#ifndef SYSTIMESETTINGDIALOG_H
#define SYSTIMESETTINGDIALOG_H

#include <QWidget>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QLabel>
#include <QStringListModel>
#include <QComboBox>
#include <QCalendarWidget>
#include <QPushButton>
#include <QDialog>
#include "NumberKeyboard.h"
#include "QMyEdit.h"

namespace UI
{

class SysTimeSettingDialog : public QDialog
{
    Q_OBJECT
public:
    SysTimeSettingDialog(QWidget *parent = 0);
    ~SysTimeSettingDialog();
    void SetTime();

protected:
    virtual void showEvent(QShowEvent *event);

private:

    QCalendarWidget *m_dayCaledar;
    QMyEdit *m_dayEdit;
    QPushButton *m_okButton;
    QPushButton *m_noButton;
    QLineEdit *m_timeEdit;

private slots:
    void ChoseMinDay();
    void ChangeMinDay();

};

}
#endif // SYSTIMESETTINGDIALOG_H
