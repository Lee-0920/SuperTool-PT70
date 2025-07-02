#ifndef FILEOPERATEWIDGET_H
#define FILEOPERATEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include "FileTableWidget.h"

#define SuperToolBakDir "/opt/SuperToolBak/"

namespace UI
{
class FileOperateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileOperateWidget(QWidget *parent = 0);
    ~FileOperateWidget();

private:

    QFileInfoList ExecuteSearch(QDir &dir);
    void ShowFiles(const QFileInfoList &files);
    void LoopSearch(QString path);
    QString GetPath();

    QLabel *m_dirLabel;
    QComboBox *m_dirComboBox;
    QPushButton *m_findButton;
    QPushButton *m_deleteAllButton;
    QPushButton *m_backupButton;
    QPushButton *m_recoverButton;
    QPushButton *m_returnButton;
    QPushButton *m_importButton;
    FileTableWidget *m_fileTable;

    QStringList m_dirList;

    int m_nCOunt;

    QWidget *m_parent;

signals:
 void NextDirSignal(QString path);

private slots:
   void ComboBoxChanged(int index);
   void SearchSlot();
   void DeleteFileSlot(int index);
   void DeleteAllSlot();
   void BackupSlot();
   void RecoverSlot();
   void ReturnSlot();
   void ImportSlot();
   void CurrentCellSlot(QTableWidgetItem* item);
   void NextDirSlot(QString path);

};
}

#endif // FILEOPERATEWIDGET_H
