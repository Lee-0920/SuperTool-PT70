#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include "FileTableWidget.h"


namespace UI
{
class ImportDailog : public QDialog
{
    Q_OBJECT
public:
    explicit ImportDailog(QString destDir, QWidget *parent = 0);
    ~ImportDailog();

private:

    QFileInfoList ExecuteSearch(QDir &dir);
    void ShowFiles(const QFileInfoList &files);
    void Search();
    void LoopSearch(QString path);

    QPushButton *m_quitButton;
    QPushButton *m_returnButton;
    FileTableWidget *m_fileTable;

    QStringList m_dirList;
    QString m_destDir;

signals:
 void NextDirSignal(QString path);

private slots:
   void ImportFileSlot(int index);
   void ReturnSlot();
   void CurrentCellSlot(QTableWidgetItem* item);
   void NextDirSlot(QString path);

};
}

#endif // IMPORTDIALOG_H
