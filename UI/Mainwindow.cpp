#include <QVBoxLayout>
#include "Flow/FlowManager.h"
#include "Mainwindow.h"
#include "UI/LoginDialog.h"
#include "checkUser.h"

using namespace Flow;

namespace UI
{
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->move(QPoint(0,0));
    this->adjustSize();
    SetupTitel();
    SetupWiget();
    SetStatusBar();

    connect(FlowManager::Instance(), SIGNAL(UpdaterWindowSignal()), this, SLOT(UpdaterWindowSlot()));
}

void MainWindow::SetupTitel()
{
    this->setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);//去掉标题栏
    this->setWindowTitle("超级工具 1.0.2");
}

void MainWindow::SetupWiget()
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setFixedSize(800, 550);
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->clear();

    if (CheckUser::Instance()->getUser() == UserType::Super)
    {
        m_fileOperateWidget = new FileOperateWidget(this);
        m_tabWidget->addTab(m_fileOperateWidget, tr("文件操作"));
    }

    m_updaterWidget = new UpdaterWidget(this);
    m_tabWidget->addTab(m_updaterWidget, tr("升级功能"));

    m_auxiliaryWidget = new AuxiliaryWidget(this);
    m_tabWidget->addTab(m_auxiliaryWidget, tr("高级功能"));

    m_linuxCmdWidget = new LinuxCmd(this);
    m_tabWidget->addTab(m_linuxCmdWidget, tr("命令行"));

    this->setCentralWidget(m_tabWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::SetStatusBar()
{
    m_statusLabel = new QLabel();
    m_statusLabel->setObjectName("blueLabel");
    QFont statusFont = m_statusLabel->font();
    statusFont.setPointSize(12);
    m_statusLabel->setFont(statusFont);
    m_statusLabel->setFixedSize(500, 40);
    m_statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_statusLabel->setText("  超级工具 1.0.2");

    m_softwareInfoLabel = new QLabel();
    m_softwareInfoLabel->setObjectName("blueLabel");
    QFont infoFont = m_softwareInfoLabel->font();
    infoFont.setPointSize(11);
    m_softwareInfoLabel->setFont(infoFont);
    m_softwareInfoLabel->setFixedSize(200, 40);
    m_softwareInfoLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    m_softwareInfoLabel->setText("");

    m_setUserBtn = new QPushButton();
    m_setUserBtn->setObjectName("brownButton");
    QFont btnFont = m_setUserBtn->font();
    btnFont.setPointSize(14);
    m_setUserBtn->setFont(btnFont);
    m_setUserBtn->setFixedSize(100, 50);
    m_setUserBtn->setText("切换用户");
    connect(m_setUserBtn, SIGNAL(clicked()), this, SLOT(SlotChangeUser()));

    m_statusBar = this->statusBar();
    m_statusBar->addWidget(m_setUserBtn, 0);
    m_statusBar->addWidget(m_statusLabel, 1);
    m_statusBar->addWidget(m_softwareInfoLabel, 2);


    m_curDateTimer = new QTimer();
    connect(m_curDateTimer, SIGNAL(timeout()), this, SLOT(SlotUpdataDateTime()));
    m_curDateTimer->start(1000);
}

void MainWindow::SlotChangeUser()
{
    this->hide();
    LoginDialog loginDialog;
    loginDialog.exec();
    this->SetupWiget();
    this->show();
}

void MainWindow::SlotUpdataDateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString currentTimeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式
    m_softwareInfoLabel->setText(currentTimeStr);
}

void MainWindow::UpdaterWindowSlot()
{
    this->update();
}

}
