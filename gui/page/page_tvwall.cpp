#include "page_tvwall.h"
#include "ui_page_tvwall.h"
#include "frmmessagebox.h"

#include "biz_config.h"
#include "page_manager.h"

page_tvWall::page_tvWall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_tvWall),
    page_dev(NULL),
    screen_cur_dec(0)
{
    ui->setupUi(this);

    page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }

    init_form();
}

page_tvWall::~page_tvWall()
{
    delete ui;
}

void page_tvWall::init_form()
{
    setupTreeWidgetScreen();
    setupTreeWidgetNvr();
    setupTableWidget();
    connect(page_dev, SIGNAL(signalDevInfoChange(SSGuiDev)), this, SLOT(refreshDevInfo(SSGuiDev)));//更新设备 alive 状态
}

void page_tvWall::refreshDevInfo(SGuiDev dev)
{
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_item;
    QList<QTreeWidgetItem *>::Iterator list_iter;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }

    DBG_PRINT("dev type: %d, ip: %s\n", dev.devicetype, qstr_ip.toUtf8().constData());

    if (dev.devicetype == EM_SWITCH_DEC)
    {
#if 0   //暂时决定不显示 icon
        list_item = ui->treeWidget_screen->findItems(qstr_ip, Qt::MatchContains);

        if (list_item.isEmpty())
        {
            DBG_PRINT("findItems screen not found\n, dev ip: %s\n", qstr_ip.toUtf8().constData());

            return ;
        }

        for (list_iter = list_item.begin(); list_iter != list_item.end(); ++list_iter)
        {
            if (dev.b_alive)
            {
                (*list_iter)->setIcon(0, QIcon(":/image/dev_online.png"));
            }
            else
            {
                (*list_iter)->setIcon(0, QIcon(":/image/dev_offline.png"));
            }
        }
#endif
    }
    else if (dev.devicetype == EM_NVR)
    {
        list_item = ui->treeWidget_nvr->findItems(qstr_ip, Qt::MatchContains);

        if (list_item.isEmpty())
        {
            DBG_PRINT("findItems nvr not found\n, dev ip: %s\n", qstr_ip.toUtf8().constData());

            return ;
        }

        for (list_iter = list_item.begin(); list_iter != list_item.end(); ++list_iter)
        {
            if (dev.b_alive)
            {
                (*list_iter)->setIcon(0, QIcon(":/image/dev_online.png"));

                //remove child
                if ((*list_iter)->childCount() != 0)
                {
                    QList<QTreeWidgetItem *> list_tmp = (*list_iter)->takeChildren();
                    QList<QTreeWidgetItem *>::Iterator iter_tmp;
                    for (iter_tmp = list_tmp.begin(); iter_tmp != list_tmp.end(); ++iter_tmp)
                    {
                        delete *iter_tmp;
                    }
                    list_tmp.clear();
                }

                //add child
                QList<QTreeWidgetItem *> list_chn;
                QTreeWidgetItem * chn_item;
                QString qstr_chn;
                int i;
                for (i = 0; i < dev.maxChnNum; ++i)
                {
                    chn_item = new QTreeWidgetItem;
                    if (NULL == chn_item)
                    {
                        ERR_PRINT("chn new QTreeWidgetItem failed\n");
                        return;
                    }

                    qstr_chn = QString("chn%1").arg(i+1);
                    chn_item->setText(0, qstr_chn);
                    chn_item->setIcon(0, QIcon(":/image/chn.png"));

                    list_chn.append(chn_item);
                }
                if (list_chn.isEmpty())
                {
                    ERR_PRINT("dev: %s, list_chn.isEmpty()\n", qstr_ip.toUtf8().constData());
                }
                else
                {
                    (*list_iter)->addChildren(list_chn);
                }
            }
            else
            {
                (*list_iter)->setIcon(0, QIcon(":/image/dev_offline.png"));
            }
        }
    }
    else
    {
        DBG_PRINT("ip: %s, dev type: %d, current page not support\n", qstr_ip.toUtf8().constData(), dev.devicetype);
    }
}

void page_tvWall::refreshTreeWidgetScreen()
{
    ui->treeWidget_screen->clear();//清空，重新载入数据

    std::vector<SConfigTvWallParam> vtvwall_list;
    if (BizConfigGetTvWallList(vtvwall_list))
    {
        ERR_PRINT("BizConfigGetTvWallList failed\n");
        return ;
    }

    if (vtvwall_list.empty())
    {
        ERR_PRINT("BizConfigGetTvWallList empty\n");
        return ;
    }

    std::vector<SConfigTvWallParam>::iterator iter = vtvwall_list.begin();
    QTreeWidgetItem *dev_item = NULL;
    QList<QTreeWidgetItem *> list_dev;

    u32 cnt = iter->ndevs_per_line * iter->ndevs_per_col;
    if (cnt != iter->devlist.size())
    {
        ERR_PRINT("rows: %d, cols: %d, devlist size: %d\n",
                  iter->ndevs_per_line,
                  iter->ndevs_per_col,
                  iter->devlist.size());
        return ;
    }

    SGuiDev dev;
    QString qstr_screen = QString::fromUtf8("屏幕");
    QString qstr_ip;
    QString qstr_item;
    struct in_addr in;
    u32 dev_ip;
    u32 i=0;

    for (i=0; i<cnt; ++i)
    {
        dev_ip = iter->devlist.at(i);
        if (dev_ip != 0)
        {
            in.s_addr = dev_ip;
            qstr_ip = QString(inet_ntoa(in));
            if (qstr_ip.isEmpty())
            {
                ERR_PRINT("dev ip invalid\n");
                continue;
            }

#if 0   //暂时决定不显示 icon
            if (page_dev->getDevInfo(EM_SWITCH_DEC, dev_ip, dev))
            {
                ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_SWITCH_DEC, inet_ntoa(in));
                continue;
            }
#endif

        }
        else
        {
            qstr_ip = QString::fromUtf8("");
        }


        dev_item = NULL;
        dev_item = new QTreeWidgetItem;
        if (NULL == dev_item)
        {
            ERR_PRINT("dev new QTreeWidgetItem failed\n");
            return;
        }

        //DBG_PRINT("dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));
        qstr_item = qstr_screen + QString("%1: ").arg(i+1) + qstr_ip;
        dev_item->setText(0, qstr_item);

#if 0   //暂时决定不显示 icon
        if (dev_ip != 0)
        {
            if (dev.b_alive)
            {
                dev_item->setIcon(0, QIcon(":/image/dev_online.png"));
            }
            else
            {
                dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
            }
        }
#endif
        list_dev.append(dev_item);
    }

    ui->treeWidget_screen->addTopLevelItems(list_dev);
}
void page_tvWall::screenItemDoubleClicked(QTreeWidgetItem* item, int col)
{
    if (col != 0)
    {
        ERR_PRINT("col: %d, invalid\n", col);
        return ;
    }

    //格式：屏幕1: 192.168.1.114，分隔符": "
    QString qstr_item = item->text(0);
    QStringList qstr_list = qstr_item.split(QString::fromUtf8(": "));
    if (qstr_list.size() != 2)
    {
        ERR_PRINT("list size: %d, invalid\n", qstr_list.size());
        return ;
    }

    QString qstr_ip = *(qstr_list.constBegin() + 1);
    if (qstr_ip.isEmpty()) //当前屏幕没有绑定解码器
    {
        DBG_PRINT("%s not binding dec\n", qstr_list.constBegin()->toUtf8().constData());
        return ;
    }

    u32 dev_ip = inet_addr(qstr_ip.toUtf8().constData());
    if (INADDR_NONE == dev_ip)
    {
        ERR_PRINT("dev ip: %s, invalid\n", qstr_ip.toUtf8().constData());
        return ;
    }

    screen_cur_dec = dev_ip;
}

void page_tvWall::setupTreeWidgetScreen()
{
    ui->treeWidget_screen->setColumnCount(1);//1列
    ui->treeWidget_screen->setHeaderLabels(QStringList() << QString::fromUtf8("电视墙屏幕列表"));

    //refreshTreeWidgetScreen();
    connect(ui->treeWidget_screen, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(screenItemDoubleClicked(QTreeWidgetItem*,int)));
}
void page_tvWall::refreshTreeWidgetNvr()
{
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator list_iter;
    SGuiDev dev;
    QTreeWidgetItem *dev_item = NULL;
    QTreeWidgetItem *chn_item = NULL;
    QList<QTreeWidgetItem *> list_dev;
    QList<QTreeWidgetItem *> list_chn;
    struct in_addr in;
    QString qstr_ip, qstr_chn;
    u32 dev_ip;
    int i;

    ui->treeWidget_nvr->clear();//清空，重新载入数据

    if (page_dev->getDevIPList(EM_NVR, dev_ip_list))
    {
        ERR_PRINT("getDevIPList failed, dev type: %d\n", EM_NVR);
        return;
    }

    for (list_iter = dev_ip_list.begin();
         list_iter != dev_ip_list.end();
         ++list_iter)
    {
        dev_ip = *list_iter;

        in.s_addr = dev_ip;
        qstr_ip = QString(inet_ntoa(in));
        if (qstr_ip.isEmpty())
        {
            ERR_PRINT("dev ip invalid\n");
            continue;
        }

        if (page_dev->getDevInfo(EM_NVR, dev_ip, dev))
        {
            ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_NVR, inet_ntoa(in));
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
        dev_item->setText(0, qstr_ip);

        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.png"));
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }

        list_dev.append(dev_item);

    #if 1
        list_chn.clear();

        for (i = 0; i < dev.maxChnNum; ++i)
        {
            chn_item = new QTreeWidgetItem;
            if (NULL == chn_item)
            {
                ERR_PRINT("chn new QTreeWidgetItem failed\n");
                return;
            }

            qstr_chn = QString(QString::fromUtf8("通道%1").arg(i+1));
            chn_item->setText(0, qstr_chn);
            chn_item->setIcon(0, QIcon(":/image/chn.png"));

            list_chn.append(chn_item);
        }
        if (list_chn.isEmpty())
        {
            DBG_PRINT("dev: %s, list_chn.isEmpty()\n", qstr_ip.toUtf8().constData());
        }
        else
        {
            dev_item->addChildren(list_chn);
        }

    #endif
    }

    ui->treeWidget_nvr->addTopLevelItems(list_dev);
}

void page_tvWall::setupTreeWidgetNvr()
{
    QStringList strlist_devtype;
    if (page_dev->getDevTypeStrList(strlist_devtype))
    {
        ERR_PRINT("getDevTypeStrList failed\n");
        return;
    }

    ui->treeWidget_nvr->setColumnCount(1);//1列
    ui->treeWidget_nvr->setHeaderLabels(QStringList() << strlist_devtype.at(EM_NVR-EM_NVR));//EM_NVR-EM_NVR

    //refreshTreeWidgetNvr();

    ui->treeWidget_nvr->setDragEnabled(true);
    ui->treeWidget_nvr->setAcceptDrops(true);
}

void page_tvWall::refreshTableWidget()
{
    ui->tableWidget_tvwall->clearContents();
    ui->tableWidget_tvwall->setRowCount(0);

    if (screen_cur_dec == 0)
    {
        return ;
    }

    struct in_addr in;
    in.s_addr = screen_cur_dec;

    SGuiDev dev;
    if (page_dev->getDevInfo(EM_SWITCH_DEC, screen_cur_dec, dev))
    {
        ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_SWITCH_DEC, inet_ntoa(in));
        ShowMessageBoxError(QString::fromUtf8("获取设备信息出错"));
        return ;
    }

    ui->tableWidget_tvwall->setRowCount(dev.maxChnNum);

    //获取解码器通道

    QString screen_text = QString::fromUtf8("屏幕通道");
    QString btn_text = QString::fromUtf8("解除绑定");
    QPushButton *btn_unbind = NULL;
    QTableWidgetItem *ptable_item = NULL;

    int i;
    for (i=0; i<dev.maxChnNum; ++i)
    {
        //屏幕(解码器)通道
        ptable_item = new QTableWidgetItem();
        ptable_item->setText(screen_text + QString("%1").arg(i+1));
        ui->tableWidget_tvwall->setItem(i, 0, ptable_item);

        //通道


        //解除绑定
        btn_unbind = new QPushButton;
        btn_unbind->setText(btn_text);

        connect(btn_unbind, SIGNAL(clicked()), this, SLOT(btn_unbind_clicked()));

        ui->tableWidget_tvwall->setCellWidget(i, 2, btn_unbind);

    }
}

void page_tvWall::btn_unbind_clicked()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(QObject::sender());//找到信号发送者
    QModelIndex index = ui->tableWidget_tvwall->indexAt(btn->pos());//定位按钮

    DBG_PRINT("btn at row: %d, col: %d\n", index.row(), index.column());

    QTableWidgetItem *item = ui->tableWidget_tvwall->item(index.row(), index.column()-1); //得到 解码器列 item
    if (item)
    {
        item->setText(QString::fromUtf8(""));//清空

        //setipc
    }
}

void page_tvWall::setupTableWidget()
{
    QStringList header;

    ui->tableWidget_tvwall->setupStyle(tablewidget_tvwall::style_chn);

    ui->tableWidget_tvwall->setColumnCount(3);
    ui->tableWidget_tvwall->setColumnWidth(0,100);
    ui->tableWidget_tvwall->setColumnWidth(1,200);
    ui->tableWidget_tvwall->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_tvwall->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_tvwall->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_tvwall->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_tvwall->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑

    //设置表头
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    ui->tableWidget_tvwall->setHorizontalHeaderItem(2, __qtablewidgetitem2);

    header.append(QString::fromUtf8("屏幕通道"));
    header.append(QString::fromUtf8("NVR通道"));
    header.append(QString::fromUtf8("操作"));
    ui->tableWidget_tvwall->setHorizontalHeaderLabels(header);

    //refreshTableWidget();

    ui->tableWidget_tvwall->setAcceptDrops(true);//拖拽效果 接受落下
}

void page_tvWall::showEvent(QShowEvent *event)
{
    DBG_PRINT("1\n");
    screen_cur_dec = 0;
    refreshTreeWidgetScreen();
    refreshTreeWidgetNvr();
    refreshTableWidget();
}







