#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MessageDialog.h"

namespace UI
{
    MessageDialog::MessageDialog(QWidget *parent) : QDialog(parent)
    {
        this->resize(300, 100);
        this->setStyleSheet("background-color: rgb(240, 250, 255);");
//        this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏

        QFont font;
        font.setPointSize(14);

        m_msgLabel = new QLabel();

        m_msgLabel->setObjectName(QStringLiteral("whiteLabel"));
        m_msgLabel->setFont(font);
        m_msgLabel->setAlignment(Qt::AlignCenter);
        m_msgLabel->setText("");
        m_msgLabel->setWordWrap(true);
        m_msgLabel->adjustSize();

        QHBoxLayout *msgHLayout = new QHBoxLayout();
        msgHLayout->addStretch();
        msgHLayout->addWidget(m_msgLabel);
        msgHLayout->addStretch();
        msgHLayout->setSpacing(0);

        QVBoxLayout *msgVLayout = new QVBoxLayout();
        msgVLayout->addStretch();
        msgVLayout->addLayout(msgHLayout);
        msgVLayout->addStretch();
        msgVLayout->setSpacing(0);

        this->setLayout(msgVLayout);
    }

    void MessageDialog::SetText(const QString &text)
    {
        m_msgLabel->setText(text);
    }
}
