#include "page_tvwall.h"
#include "ui_page_tvwall.h"
#include "frmmessagebox.h"

#include "biz_config.h"
#include "page_manager.h"

#include <stdio.h>

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
    QMutexLocker locker(&mutex);
    connect(page_dev, SIGNAL(signalDevInfoChange(SGuiDev)), this, SLOT(refreshDevInfo(SGuiDev)));//更新设备 alive 状态

    refreshPatrolControl();
    setupTreeWidgetScreen();
    setupTreeWidgetNvr();
    setupTableWidget();
}

void page_tvWall::refreshPatrolControl()
{
    u32 dev_ip = screen_cur_dec;
    struct in_addr in;
    in.s_addr = dev_ip;

    if (dev_ip == 0)
    {
        ui->gbox_patrol->setEnabled(false);
        return ;
    }

    int ret = SUCCESS;
    u32 buf_size = 256;
    char buf[256];
    ifly_patrol_para_t *para = (ifly_patrol_para_t *)buf;

    ret = BizDevGetPatrolPara(EM_SWITCH_DEC, dev_ip, para, &buf_size);
    if (ret)
    {
        ERR_PRINT("dev(%s) BizDevGetPatrolPara failed, ret: %d\n", inet_ntoa(in), ret);

        ShowMessageBoxError(QString::fromUtf8("获取设备信息失败！"));
        return ;
    }

    int i;
#if 0
    printf("\t nIsPatrol: %d\n", para->nIsPatrol);
    printf("\t nInterval: %d\n", para->nInterval);
    printf("\t nPatrolMode: %d\n", para->nPatrolMode);
    printf("\t nPatrolChnNum: %d\n", para->nPatrolChnNum);
    printf("\t nInterval_num: %d", para->nInterval_num);
    for (i=0; i<para->nInterval_num; ++i)
        printf(", %d", para->value[i]);
    printf("\n");

    printf("\t nPatrolMode_num: %d", para->nPatrolMode_num);
    for (i=0; i<para->nPatrolMode_num; ++i)
        printf(", %d", para->value[i+para->nInterval_num]);
    printf("\n");
#endif

    ui->gbox_patrol->setEnabled(true);
}

void page_tvWall::refreshDevInfo(SGuiDev dev)
{
    QMutexLocker locker(&mutex);
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_dev_find;
    //QList<QTreeWidgetItem *>::Iterator list_iter;

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
        list_item = ui->treeWidget_nvr->findItems(qstr_ip, Qt::MatchContains);

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
        list_dev_find = ui->treeWidget_nvr->findItems(qstr_ip, Qt::MatchContains);
        int cnt = list_dev_find.size();
        if (1 != cnt)//查找结果必须 == 1
        {
            ERR_PRINT("findItems failed\n, list count: %d, dev ip: %s\n", cnt, qstr_ip.toUtf8().constData());

            return ;
        }

        QTreeWidgetItem *dev_item = list_dev_find.at(0);
        ui->treeWidget_nvr->deleteItemChildren(dev_item);//销毁所有通道条目

        if (dev.b_alive)
        {
            dev_item->setIcon(0, QIcon(":/image/dev_online.png"));

            //add chn item
            int ret = 0;
            char chn_name[32];
            QString qstr_chn;
            QTreeWidgetItem *chn_item = NULL;
            QList<QTreeWidgetItem *> list_chn;

            for (int i = 0; i < dev.maxChnNum; ++i)
            {
                chn_item = NULL;
                chn_item = new QTreeWidgetItem;
                if (NULL == chn_item)
                {
                    ERR_PRINT("chn new QTreeWidgetItem failed\n");
                    break ;
                }

                qstr_chn = QString(QString::fromUtf8("通道%1").arg(i+1));

                ret = BizGetDevChnName(EM_NVR, dev.deviceIP, i, chn_name, sizeof(chn_name));
                if (ret)
                {
                    ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
                }
                else
                {
                    //NVR:192.168.1.248:通道1:大门
                    if (chn_name[0]) //不为空
                    {
                        qstr_chn += QString(QString::fromUtf8(":%1").arg(QString::fromUtf8(chn_name)));
                    }
                }

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
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
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
        qstr_item = qstr_screen + QString("%1:").arg(i+1) + qstr_ip;
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
void page_tvWall::screenItemClicked(QTreeWidgetItem* item, int col)
{
    int ret = SUCCESS;
    screen_cur_dec = 0;
    refreshPatrolControl();

    if (col != 0)
    {
        ERR_PRINT("col: %d, invalid\n", col);
        return ;
    }

    clearTableWidget();

    //格式：屏幕1: 192.168.1.114，分隔符": "
    QString qstr_item = item->text(0);
    QStringList qstr_list = qstr_item.split(QString::fromUtf8(":"));
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
    refreshPatrolControl();

    //refreshTableWidget
    //获取解码器通道信息
    SGuiDev dev;
    struct in_addr in;
    in.s_addr = screen_cur_dec;

    if (page_dev->getDevInfo(EM_SWITCH_DEC, screen_cur_dec, dev))
    {
        ERR_PRINT("getDevInfo failed, dev type: %d, ip: %s\n", EM_SWITCH_DEC, inet_ntoa(in));
        return ;
    }

    ifly_ipc_info_t * pipc_info = NULL;
    pipc_info = new ifly_ipc_info_t[dev.maxChnNum];

    ret = BizGetDevChnIPCInfo(EM_SWITCH_DEC, screen_cur_dec, pipc_info, dev.maxChnNum*sizeof(ifly_ipc_info_t));
    if (ret)
    {
        ERR_PRINT("BizGetDevChnIPCInfo failed: %d, dev type: %d, ip: %s\n", ret, EM_SWITCH_DEC, inet_ntoa(in));

        delete[] pipc_info;
        return ;
    }

    refreshTableWidget(pipc_info, dev.maxChnNum);
#if 0
    //DBG_PRINT("dev(%s) :\n", inet_ntoa(in));
    u32 i;
    for (i=0; i<dev.maxChnNum; ++i)
    {
        in.s_addr = pipc_info[i].dwIp;
        DBG_PRINT("chn%d: enable: %d, protocol type: %d, nvr ip: %s, req_nvr_chn: %d\n",
                  pipc_info[i].channel_no,
                  pipc_info[i].enable,
                  pipc_info[i].protocol_type,
                  inet_ntoa(in),
                  pipc_info[i].req_nvr_chn);
    }
#endif

    delete[] pipc_info;
}

void page_tvWall::setupTreeWidgetScreen()
{
    ui->treeWidget_screen->setColumnCount(1);//1列
    ui->treeWidget_screen->setHeaderLabels(QStringList() << QString::fromUtf8("电视墙屏幕列表"));

    //refreshTreeWidgetScreen();
    connect(ui->treeWidget_screen, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(screenItemClicked(QTreeWidgetItem*,int)));
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
    int ret = SUCCESS;
    char chn_name[32];

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

            ret = BizGetDevChnName(EM_NVR, dev_ip, i, chn_name, sizeof(chn_name));
            if (ret)
            {
                ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
            }
            else
            {
                //NVR:192.168.1.248:通道1:大门
                if (chn_name[0]) //不为空
                {
                    qstr_chn += QString(QString::fromUtf8(":%1").arg(QString::fromUtf8(chn_name)));
                }
            }

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

void page_tvWall::refreshTableWidget(ifly_ipc_info_t * pipc_info, s32 ipc_nums)
{
    clearTableWidget();

    if (NULL == pipc_info || ipc_nums <= 0)
    {
        ERR_PRINT("param invalid\n");
        return ;
    }

    ui->tableWidget_tvwall->setRowCount(ipc_nums);

    QString screen_text = QString::fromUtf8("屏幕通道");
    QString chn = QString::fromUtf8(""); /*空*/
    QString btn_text = QString::fromUtf8("解除绑定");
    QPushButton *btn_unbind = NULL;
    QTableWidgetItem *ptable_item = NULL;
    char chn_name[32];
    int ret = SUCCESS;
    struct in_addr in;
    in.s_addr = screen_cur_dec;
    DBG_PRINT("switch dec ip: %s\n", inet_ntoa(in));

    for (s32 i=0; i<ipc_nums; ++i)
    {
        //屏幕(解码器)通道
        ptable_item = new QTableWidgetItem();
        ptable_item->setText(screen_text + QString("%1").arg(i+1));
        ui->tableWidget_tvwall->setItem(i, 0, ptable_item);

        //通道
        chn = QString::fromUtf8(""); /*空*/
        if (pipc_info[i].enable)
        {
            if (0x100 == pipc_info[i].protocol_type) //nvr
            {
                in.s_addr = pipc_info[i].dwIp;
                if (NULL == inet_ntoa(in))
                {
                    ERR_PRINT("pipc_info[%d].ip invalid\n", i);
                }
                else
                {
                    ret = BizGetDevChnName(EM_NVR, pipc_info[i].dwIp, pipc_info[i].req_nvr_chn, chn_name, sizeof(chn_name));
                    if (ret)
                    {
                        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
                    }
                    else
                    {
                        //192.168.1.248:通道1:大门
                        chn = QString(QString::fromUtf8("%1").arg(QString::fromUtf8(inet_ntoa(in))));
                        chn += QString(QString::fromUtf8(":通道%1").arg(pipc_info[i].req_nvr_chn+1));
                        chn += QString(QString::fromUtf8(":%1").arg(QString::fromUtf8(chn_name)));
                    }
                }
            }
        }
        ptable_item = new QTableWidgetItem();
        ptable_item->setText(chn);
        ui->tableWidget_tvwall->setItem(i, 1, ptable_item);

        //解除绑定
        btn_unbind = new QPushButton;
        btn_unbind->setText(btn_text);

        connect(btn_unbind, SIGNAL(clicked()), this, SLOT(btn_unbind_clicked()));

        ui->tableWidget_tvwall->setCellWidget(i, 2, btn_unbind);

    }
}

void page_tvWall::clearTableWidget()
{
    ui->tableWidget_tvwall->clearContents();
    ui->tableWidget_tvwall->setRowCount(0);
}

void page_tvWall::btn_unbind_clicked()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(QObject::sender());//找到信号发送者
    QModelIndex index = ui->tableWidget_tvwall->indexAt(btn->pos());//定位按钮

    int row = index.row();
    int col = index.column();
    DBG_PRINT("btn at row: %d, col: %d\n", row, col);

    QTableWidgetItem *item = ui->tableWidget_tvwall->item(index.row(), index.column()-1); //得到 解码器列 item
    if (item)
    {
        item->setText(QString::fromUtf8(""));//清空

        slotDropEvent(row, col, QString::fromUtf8(""));
    }
}

void page_tvWall::slotDropEvent(int row, int col, QString data)
{
    DBG_PRINT("row: %d, col: %d, data: %s\n", row, col, data.toUtf8().constData());

    int ret = SUCCESS;
    int screen_chn = row;//屏幕通道 即 解码器通道

    QTableWidgetItem *item = ui->tableWidget_tvwall->item(row, 1); //得到 解码器列 item
    if (NULL == item)
    {
        ERR_PRINT("NULL == item\n");
        return ;
    }

    //解除绑定
    if (data.isEmpty())
    {
        item->setText(QString::fromUtf8("")); //清空

        ret = BizDelDevChnIpc(EM_SWITCH_DEC, screen_cur_dec, screen_chn);//删除通道IPC
        if (ret)
        {
            ERR_PRINT("BizDelDevChnIpc failed, ret: %d\n", ret);
        }
        return ;
    }

    //check QString data
    QStringList str_list = data.split(QString::fromUtf8(":"));
    if (str_list.size() < 2)
    {
        ERR_PRINT("str_list size(%d) < 2, invalid\n", str_list.size());
        return ;
    }

    //req nvr
    QString str_nvr_ip = str_list.at(0);
    u32 nvr_ip = inet_addr(str_nvr_ip.toUtf8().constData());
    if (INADDR_NONE == nvr_ip)
    {
        ERR_PRINT("nvr ip invalid, str_nvr_ip: %s\n", str_nvr_ip.toUtf8().constData());
        return ;
    }

    //req nvr chn
    QString str_chn = str_list.at(1);
    if (!str_chn.contains(QString::fromUtf8("通道"), Qt::CaseInsensitive))//不含有"通道"
    {
        ERR_PRINT("chn str: %s, contains invalid\n", str_chn.toUtf8().constData());
        return ;
    }

    u32 nvr_chn = 0xff;
    if (1 != sscanf(str_chn.toUtf8().constData(), "通道%d", &nvr_chn))
    {
        ERR_PRINT("chn str: %s, sscanf invalid\n", str_chn.toUtf8().constData());
        return ;
    }
    nvr_chn -= 1;

    ret = BizSetDevChnIpc(EM_SWITCH_DEC, screen_cur_dec, screen_chn, nvr_ip, nvr_chn);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
    }

    item->setText(data);
}

void page_tvWall::setupTableWidget()
{
    QStringList header;

    ui->tableWidget_tvwall->setupStyle(tablewidget_tvwall::style_chn);

    ui->tableWidget_tvwall->setColumnCount(3);
    ui->tableWidget_tvwall->setColumnWidth(0,100);
    ui->tableWidget_tvwall->setColumnWidth(1,320);
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

    clearTableWidget();

    ui->tableWidget_tvwall->setAcceptDrops(true);//拖拽效果 接受落下

    connect(ui->tableWidget_tvwall, SIGNAL(signalDropEvent(int,int,QString)),
            this, SLOT(slotDropEvent(int,int,QString)));
}

void page_tvWall::showEvent(QShowEvent *event)
{
    //DBG_PRINT("1\n");
    screen_cur_dec = 0;
    refreshPatrolControl();
    refreshTreeWidgetScreen();
    refreshTreeWidgetNvr();
    clearTableWidget();
}







