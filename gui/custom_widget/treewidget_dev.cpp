#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QtGui>

#include "treewidget_dev.h"
#include "gui_dev.h"

#include "page_manager.h"
#include "page_dev_mgt.h"

treewidget_dev::treewidget_dev(QWidget *parent) :
    QTreeWidget(parent),
    dev_type(EM_DEV_TYPE_NONE),
    level_nums(NONE_LEVEL)
{
    page_dev_mgt * page_dev = NULL;
    page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }

    if (page_dev->getDevTypeStrList(strlist_devtype))
    {
        ERR_PRINT("getDevTypeStrList failed\n");
        return;
    }
}

treewidget_dev::~treewidget_dev()
{

}

void treewidget_dev::setupForm(LEVELNUMS levels)
{
    if (levels <= NONE_LEVEL
            || levels >= MAX_LEVELS)
    {
        ERR_PRINT("levels(%d) not support\n");
        return ;
    }

    level_nums = levels;
}

void treewidget_dev::setupDevType(EM_DEV_TYPE _dev_type)
{
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator list_iter;
    SGuiDev dev;
    page_dev_mgt * page_dev = NULL;
    QTreeWidgetItem *dev_item = NULL;
    QTreeWidgetItem *chn_item = NULL;
    QList<QTreeWidgetItem *> list_dev;
    QList<QTreeWidgetItem *> list_chn;
    struct in_addr in;
    QString qstr;
    u32 dev_ip;
    int i = 0;

    dev_type = _dev_type;

    if (level_nums <= NONE_LEVEL
            || level_nums >= MAX_LEVELS)
    {
        ERR_PRINT("levels(%d) not support\n");
        return ;
    }

    switch (dev_type)
    {
    case EM_NVR:
        setHeaderLabels(QStringList() << QString::fromUtf8("NVR"));
        break;

    case EM_PATROL_DEC:
        setHeaderLabels(QStringList() << QString::fromUtf8("轮巡型解码器"));
        break;

    case EM_SWITCH_DEC:
        setHeaderLabels(QStringList() << QString::fromUtf8("切换型解码器"));
        break;

    default:
        ERR_PRINT("dev_type%d exception\n", dev_type);
        return;
    }

    setColumnCount(1);//1列

    page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }

    connect(page_dev, SIGNAL(signalDevInfoChange(SGuiDev)), this, SLOT(refreshDevInfo(SGuiDev)));

    if (page_dev->getDevIPList(dev_type, dev_ip_list))
    {
        ERR_PRINT("getDevIPList failed, dev type: %d\n", dev_type);
        return;
    }

    for (list_iter = dev_ip_list.begin();
         list_iter != dev_ip_list.end();
         ++list_iter)
    {
        dev_ip = *list_iter;

        in.s_addr = dev_ip;
        qstr = QString(inet_ntoa(in));
        if (qstr.isNull())
        {
            ERR_PRINT("dev ip invalid\n");
            continue;
        }

        if (page_dev->getDevInfo(dev_type, dev_ip, dev))
        {
            ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));
            continue;
        }

        dev_item = NULL;
        dev_item = new QTreeWidgetItem;
        if (NULL == dev_item)
        {
            ERR_PRINT("dev new QTreeWidgetItem failed\n");
            return;
        }

        //DBG_PRINT("dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));
        dev_item->setText(0, qstr);

        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.png"));
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }

        list_dev.append(dev_item);
        if (level_nums == TWO_LEVELS)
        {
            list_chn.clear();
            for (i = 0; i < dev.maxChnNum; ++i)
            {
                chn_item = new QTreeWidgetItem;
                if (NULL == chn_item)
                {
                    ERR_PRINT("chn new QTreeWidgetItem failed\n");
                    return;
                }

                qstr = QString("chn%1").arg(i+1);
                chn_item->setText(0, qstr);
                chn_item->setIcon(0, QIcon(":/image/chn.png"));

                list_chn.append(chn_item);
            }
            if (list_chn.isEmpty())
            {
                DBG_PRINT("list_chn.isEmpty() \n");
            }
            else
            {
                dev_item->addChildren(list_chn);
            }
        }
    }

    addTopLevelItems(list_dev);
}

void treewidget_dev::refreshDevInfo(SGuiDev dev)
{
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_item;
    int cnt = 0;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isNull())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }

    list_item = findItems(qstr_ip, Qt::MatchExactly);
    cnt = list_item.count();
    if (cnt == 0 || cnt > 1)
    {
        ERR_PRINT("findItems failed\n, list count: %d\n", cnt);
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

void treewidget_dev::startDrag(Qt::DropActions supportedActions)
{
    DBG_PRINT("treewidget_dev::startDrag\n");
    QTreeWidgetItem *pitem = NULL;//父节点
    QTreeWidgetItem *item = NULL;
    QMimeData *mimeData = NULL;
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    item = currentItem();
    if (NULL == item)
    {
        DBG_PRINT("NULL == item\n");
        return ;
    }

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

    DBG_PRINT("mime str: %s\n", mime_str.toUtf8().constData());

    dataStream << mime_str.toUtf8();

    mimeData = new QMimeData;
    mimeData->setData("network_keyboard", itemData);

    QPixmap pixmap = item->icon(0).pixmap(QSize(16, 16));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);
    drag->exec(Qt::MoveAction);
}

void treewidget_dev::dragEnterEvent(QDragEnterEvent *event)
{
    //DBG_PRINT(" 1\n");
    if (event->mimeData()->hasFormat("network_keyboard"))
    {
        //DBG_PRINT(" 2\n");
        event->accept();
    }
}

void treewidget_dev::dragLeaveEvent(QDragLeaveEvent *event)
{
    //DBG_PRINT(" 1\n");
}

void treewidget_dev::dragMoveEvent(QDragMoveEvent *event)
{
    //DBG_PRINT(" 1\n");
    if (event->mimeData()->hasFormat("network_keyboard"))
    {
        //DBG_PRINT(" 2\n");
        event->accept();
    }
}

void treewidget_dev::dropEvent(QDropEvent *event)
{
    //DBG_PRINT(" 1\n");
    if (event->mimeData()->hasFormat("network_keyboard"))
    {
        //DBG_PRINT(" 2\n");
        event->accept();
    }
}

























