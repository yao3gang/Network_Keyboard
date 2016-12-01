#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void treewidget_dev::setupStyle(EM_DEV_TYPE _dev_type)
{
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator list_iter;
    SGuiDev dev;
    page_dev_mgt * page_dev = NULL;
    QTreeWidgetItem *dev_item = NULL;
    QTreeWidgetItem *chn_item = NULL;
    struct in_addr in;
    QString qstr;
    u32 dev_ip;
    int i = 0;

    dev_type = _dev_type;
    if (dev_type <= EM_DEV_TYPE_NONE
            || dev_type >= EM_DEV_TYPE_MAX)
    {
        ERR_PRINT("dev_type(%d) not support\n", dev_type);
        return;
    }

    setColumnCount(1);//1列

    page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }

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
        dev_item = new QTreeWidgetItem(this);
        if (NULL == dev_item)
        {
            ERR_PRINT("new QTreeWidgetItem failed\n");
            return;
        }

        dev_item->setText(0, qstr);
        dev_item->setIcon(0, QIcon(":/image/test_2.ico"));
#if 0
        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.ico"));
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.ico"));
        }
#endif
    }
}
