#ifndef UI_FRAME_FILETABLEWIDGET_H
#define UI_FRAME_FILETABLEWIDGET_H
#include <QtGui>
#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>


namespace UI
{
class FileTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit FileTableWidget(QWidget *parent = 0);
    void ClearContent();
    void SetFixedSize(int w, int h);

private:


signals:
    void ClearContentSignal();
};
}
#endif // UI_FRAME_FILETABLEWIDGET_H
