#ifndef UpdaterWidget_H
#define UpdaterWidget_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include "MessageDialog.h"
#include "FileTableWidget.h"

namespace UI
{
class UpdaterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UpdaterWidget(QWidget *parent = 0);
    ~UpdaterWidget();

private:

    QFileInfoList ExecuteSearch(QDir &dir);
    void ShowFiles(const QFileInfoList &files);

    QLabel *m_kernelLabel;
    QLabel *m_kVersionLabel;
    QLabel *m_fileSystemLabel;
    QLabel *m_fsVersionLabel;

    QPushButton *m_findButton;

    FileTableWidget *m_fileTable;

    int m_nCOunt;

    MessageDialog *m_msgDialog;
signals:

private slots:

   void OnSearch();
   void OnUpdater(int index);



};
}

#endif // UpdaterWidget_H
