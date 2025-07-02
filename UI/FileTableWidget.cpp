#include <QDebug>
#include "FileTableWidget.h"


namespace UI
{
FileTableWidget::FileTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    this->horizontalHeader()->setSectionsClickable(false); //设置表头不可点击（默认点击后进行排序）
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setHighlightSections(false); // 点击表时不对表头行光亮（获取焦点）
//    this->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(255, 255, 245);}");
    QFont dataFont = this->font();
    dataFont.setPointSize(12);
    this->setFont(dataFont);
    this->verticalHeader()->setVisible(false); // 隐藏列表头
    this->setFrameShape(QFrame::StyledPanel); //设置无边框
    this->setShowGrid(true); //设置不显示格子线
//    this->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色

    //设置水平滚动条样式
    this->horizontalScrollBar()->setStyleSheet
            ("QScrollBar{background:transparent; height:1px;}"
             "QScrollBar::handle{background:lightgray; border:1px solid transparent; border-radius:1px;}"
             "QScrollBar::handle:hover{background:gray;}"
             "QScrollBar::sub-line{background:transparent;}"
             "QScrollBar::add-line{background:transparent;}");
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);// 设置不可拖拽
    this->horizontalScrollBar()->setVisible(false);
    this->horizontalScrollBar()->setDisabled(true);

    this->setColumnCount(3);

    QFont headerFont;
    headerFont.setPointSize(12);

    this->horizontalHeader()->setFont(headerFont);

    this->horizontalHeader()->setFixedHeight(25); // 设置表头的高度
    this->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 25px;}");

    this->setEditTriggers(QAbstractItemView::NoEditTriggers); // 将表格变为禁止编辑

    this->setColumnWidth(0, 510);
    this->setColumnWidth(1, 160);
    this->setColumnWidth(2, 80);

    QStringList headerStr;
    headerStr << "文件名" << "大小"<< "";

    for(int i = 0;i< headerStr.size();i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(headerStr.at(i));
        item->setFont(headerFont);
        this->setHorizontalHeaderItem(i, item);
    }

    connect(this, SIGNAL(ClearContentSignal()), this, SLOT(clearContents()));
}

void FileTableWidget::SetFixedSize(int w, int h)
{
    this->setFixedSize(w, h);
}

void FileTableWidget::ClearContent()
{
    emit ClearContentSignal();
}


}
