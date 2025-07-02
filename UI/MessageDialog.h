#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <QString>
#include <QLabel>

namespace UI
{

class MessageDialog : public QDialog
{
public:
    MessageDialog(QWidget *parent = 0);
    void SetText(const QString &text);
private:
    QLabel *m_msgLabel;
};
}
#endif // MESSAGEDIALOG_H
