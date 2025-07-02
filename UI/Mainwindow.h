#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include "FileOperateWidget.h"
#include "AuxiliaryWidget.h"
#include "UpdaterWidget.h"
#include "QPushButton"
#include "LinuxCmd.h"

namespace UI
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void SetupTitel();
    void SetupWiget();
    void SetStatusBar();

    ~MainWindow();

private:
    QLabel *m_statusLabel;
    QLabel *m_softwareInfoLabel;
    QPushButton *m_setUserBtn;
    QStatusBar* m_statusBar;
    QTabWidget *m_tabWidget;
    FileOperateWidget *m_fileOperateWidget;
    UpdaterWidget *m_updaterWidget;
    AuxiliaryWidget *m_auxiliaryWidget;
    LinuxCmd *m_linuxCmdWidget;
    QTimer *m_curDateTimer;

private slots:

void SlotUpdataDateTime();
void SlotChangeUser();
void UpdaterWindowSlot();

};
}
#endif // MAINWINDOW_H
