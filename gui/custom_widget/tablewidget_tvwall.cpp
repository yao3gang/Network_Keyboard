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
        event->setDropAction(Qt::MoveAction);
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
    //DBG_PRINT("drop at row: %d, col: %d\n", index.row(), index.column());
    int row = index.row();
    int col = index.column();
#if 0
    QTableWidgetItem *ptable_item = item(index.row(), 1); //得到 解码器列 item
    if (!ptable_item)
    {
        event->ignore();
        return ;
    }
#endif

    QByteArray data = event->mimeData()->data("network_keyboard");
    QDataStream dataStream(&data, QIODevice::ReadOnly);
    char *pstr = NULL;
    dataStream >> pstr;
    if (pstr)
    {
    #if 1
        QString str = QString::fromUtf8(pstr);

        event->setDropAction(Qt::MoveAction);
        event->accept();
        delete[] pstr;

        emit signalDropEvent(row, col, str);
    #else
        QString item_str = QString::fromUtf8(pstr);
        DBG_PRINT("item_str: %s\n", item_str.toUtf8().constData());

        QStringList str_list = item_str.split(QString::fromUtf8(":"));

        DBG_PRINT("str_list size: %d\n", str_list.size());
        int i;
        for (i=0; i<str_list.size(); ++i)
        {
            DBG_PRINT("%d: %s\n", i, str_list.at(i).toUtf8().constData());
        }

        if (style == style_screen)
        {
            if (str_list.size() > 1)
            {
                ERR_PRINT("str_list size(%d) > 1, invalid\n", str_list.size());
                goto fail;
            }
        }
        else if (style == style_chn)
        {
            if (str_list.size() < 2)
            {
                ERR_PRINT("str_list size(%d) < 2, invalid\n", str_list.size());
                goto fail;
            }
        }
        else
        {
            ERR_PRINT("style: %s, not support\n", style.toUtf8().constData());

            delete[] pstr;
            event->ignore();
        }

        if (item_str != ptable_item->text())
        {
            ptable_item->setText(item_str);
            emit bindChanged(ptable_item->row());
        }

        event->setDropAction(Qt::MoveAction);
        event->accept();

        delete[] pstr;
        return ;

fail:
        delete[] pstr;
        event->ignore();
    #endif
    }
    else
    {
        event->ignore();
    }
}
