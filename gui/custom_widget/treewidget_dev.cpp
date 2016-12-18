#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QtGui>

#include "treewidget_dev.h"
#include "gui_dev.h"

#include "page_manager.h"
#include "page_dev_mgt.h"

treewidget_dev::treewidget_dev(QWidget *parent) :
    QTreeWidget(parent)
{

}

treewidget_dev::~treewidget_dev()
{

}

void treewidget_dev::deleteItemChildren(QTreeWidgetItem *item)
{
    if (NULL == item)
    {
        return ;
    }

    DBG_PRINT("children count: %d\n", item->childCount());
#if 1
    QTreeWidgetItem *child = NULL;
    while (item->childCount())
    {
        child = item->takeChild(0);
        if (child)
        {
            delete child;
            child = NULL;
        }
        else
        {
            ERR_PRINT("child == NULL\n");
        }
    }
#endif
}

#if 0
void treewidget_dev::refreshDevInfo(SGuiDev dev)
{
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_item;
    int cnt = 0;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }

    list_item = findItems(qstr_ip, Qt::MatchContains);
    cnt = list_item.count();
    if (cnt == 0 || cnt > 1)
    {
        ERR_PRINT("findItems failed\n, list count: %d, dev ip: %s\n", cnt, qstr_ip.toUtf8().constData());

#if 1
        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            DBG_PRINT("item text: %s\n", (*it)->text(0).toUtf8().constData());
            ++it;
        }
#endif
        return ;
    }

    if (dev.b_alive)
    {
        (*list_item.begin())->setIcon(0, QIcon(":/image/dev_online.png"));
    }
    else
    {
        (*list_item.begin())->setIcon(0, QIcon(":/image/dev_offline.png"));
    }
}
#endif

void treewidget_dev::startDrag(Qt::DropActions supportedActions)
{
    DBG_PRINT("treewidget_dev::startDrag\n");
    //QTreeWidgetItem *pitem = NULL;//父节点
    QTreeWidgetItem *item = NULL;
    QMimeData *mimeData = NULL;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    QString str_tmp, mime_str;

    item = currentItem();
    if (NULL == item)
    {
        DBG_PRINT("NULL == item\n");
        return ;
    }

    while (item)
    {
        if (mime_str.isEmpty())
        {
            str_tmp = item->text(0);
        }
        else
        {
            str_tmp = item->text(0) + QString::fromUtf8(":") + mime_str;
        }
        mime_str = str_tmp;

        item = item->parent();
    }

#if 0
    pitem = item->parent();
    QString item_text = item->text(0);
    QString mime_str;

    DBG_PRINT("item text: %s\n", item_text.toUtf8().constData());

    if (NULL == item->parent()) //根节点
    {
        if (INADDR_NONE == inet_addr(item_text.toUtf8().constData()))//IP 地址无效
        {
            ERR_PRINT("dev ip invalid\n");
            return ;
        }
        mime_str = item_text;
    }
    else if (item_text.contains(QString::fromUtf8("chn"), Qt::CaseInsensitive))//通道子节点
    {
        if (NULL == pitem)
        {
            ERR_PRINT("chn item parent = NULL\n");
            return ;
        }

        if (INADDR_NONE == inet_addr(pitem->text(0).toUtf8().constData()))//IP 地址无效
        {
            ERR_PRINT("dev ip invalid\n");
            return ;
        }
        //格式  ip:chnx
        mime_str = pitem->text(0);
        mime_str.append(QString::fromUtf8(":"));
        mime_str.append(item_text);
    }
    else
    {
        ERR_PRINT("item text: %s, not support\n", item_text.toUtf8().constData());
        return ;
    }
#endif

    DBG_PRINT("mime str: %s\n", mime_str.toUtf8().constData());

    dataStream << mime_str.toUtf8().constData();
    mimeData = new QMimeData;
    mimeData->setData("network_keyboard", itemData);

    QPixmap pixmap = currentItem()->icon(0).pixmap(QSize(16, 16));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
    drag->exec(Qt::MoveAction);
}

void treewidget_dev::dragEnterEvent(QDragEnterEvent *event)
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

void treewidget_dev::dragLeaveEvent(QDragLeaveEvent *event)
{
    //DBG_PRINT(" 1\n");
    event->accept();
}

void treewidget_dev::dragMoveEvent(QDragMoveEvent *event)
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

void treewidget_dev::dropEvent(QDropEvent *event)
{
    //DBG_PRINT(" 1\n");
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

























