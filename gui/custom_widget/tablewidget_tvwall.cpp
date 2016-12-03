#include "tablewidget_tvwall.h"

const QString tablewidget_tvwall::style_screen = QString::fromUtf8("screen_binding");
const QString tablewidget_tvwall::style_chn = QString::fromUtf8("chn_binding");;

tablewidget_tvwall::tablewidget_tvwall(QWidget *parent) :
    QTableWidget(parent)
{

}

tablewidget_tvwall::~tablewidget_tvwall()
{

}

void tablewidget_tvwall::setupStyle(QString str)
{
    //QStringList header;
    style = str;
}

void tablewidget_tvwall::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("network_keyboard"))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void tablewidget_tvwall::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void tablewidget_tvwall::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("network_keyboard"))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void tablewidget_tvwall::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("network_keyboard"))
    {
        event->ignore();
        return ;
    }

    QModelIndex index = indexAt(event->pos());
    DBG_PRINT("drop at row: %d, col: %d\n", index.row(), index.column());

    QTableWidgetItem *ptable_item = item(index.row(), 1); //得到 解码器列 item
    if (!ptable_item)
    {
        event->ignore();
        return ;
    }

    QByteArray data = event->mimeData()->data("network_keyboard");
    DBG_PRINT("mime data: %d\n", data.size());
    QString item_str = QString::fromUtf8(data);
    DBG_PRINT("item_str: %s\n", item_str.toUtf8().constData());
    DBG_PRINT("item_str: %c\n", *(item_str.toUtf8().constData()));
    ptable_item->setText(item_str);

    event->accept();
}
