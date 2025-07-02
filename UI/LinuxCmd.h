#ifndef LINUXCMD_H
#define LINUXCMD_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>

class LinuxCmd : public QWidget
{
    Q_OBJECT
public:
    explicit LinuxCmd(QWidget *parent = 0);

signals:

public slots:
    void SlotsSend();
    void SlotsOpenKey();
    void SlotsOpenLog();

private:
    QTextBrowser *m_ReadWindow;
    QTextEdit *m_WriteWindow;
    QComboBox *m_logGetLine;
};

#endif // LINUXCMD_H
