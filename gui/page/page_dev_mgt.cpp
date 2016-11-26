#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QDebug>
#include <QCursor>
#include <string>

#include "page_dev_mgt.h"
#include "ui_page_dev_mgt.h"
#include "biz_device.h"

page_dev_mgt::page_dev_mgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::page_dev_mgt),
    b_inited(false)
{
    ui->setupUi(this);
    init_form();
    init_data();
}

page_dev_mgt::~page_dev_mgt()
{
    delete ui;
}

void page_dev_mgt::init_form()//控件
{
    strlist_devtype.append(QString::fromUtf8("NVR"));
    strlist_devtype.append(QString::fromUtf8("轮巡型解码器"));
    strlist_devtype.append(QString::fromUtf8("切换型解码器"));

    ui->cmb_srh_type->addItems(strlist_devtype);
    ui->cmb_srh_type->setCurrentIndex(0);

    ui->cmb_added_type->addItems(strlist_devtype);
    ui->cmb_added_type->setCurrentIndex(0);

    //ui->tableWidget_srh->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_srh->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_srh->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_srh->setSelectionMode(QAbstractItemView::MultiSelection);//可以多选行
    ui->tableWidget_srh->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_srh->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_srh->setColumnWidth(0,50);
    ui->tableWidget_srh->setColumnWidth(1,200);
    ui->tableWidget_srh->setColumnWidth(2,200);
    //ui->tableWidget_srh->setDragEnabled(true);//设置内部部件支持拖拽
    //ui->tableWidget_srh->setAcceptDrops(true);
    //ui->tableWidget_srh->setDropIndicatorShown(false);

    //ui->tableWidget_dev->verticalHeader()->setVisible(false);//列表头不可见
    ui->tableWidget_dev->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，删除第一行
    ui->tableWidget_dev->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_dev->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableWidget_dev->setAlternatingRowColors(true);
    ui->tableWidget_dev->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_dev->setColumnWidth(0,50);
    ui->tableWidget_dev->setColumnWidth(1,200);
    ui->tableWidget_dev->setColumnWidth(2,200);
    ui->tableWidget_dev->setColumnWidth(3,100);

    connect(ui->cmb_srh_type, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbSrhChange(int)));
    connect(ui->cmb_added_type, SIGNAL(currentIndexChanged(int)), this, SLOT(cmbAddChange(int)));
    connect(ui->tableWidget_srh, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(tableWidgetSrhDBClicked(int, int)));
}

void page_dev_mgt::init_data()//初始化设备信息
{
    int i, row;
    int dev_type;
    struct in_addr in;
    std::list<u32> dev_ip_list;
    std::list<u32>::iterator iter;
    MAP_IP_DEV *pmap = NULL;
    MAP_IP_DEV::iterator map_iter;
    SGuiDev *pdev = NULL;

    QMutexLocker locker(&mutex);

    qRegisterMetaType<SGuiDev>("SGuiDev");
    connect(gp_bond, SIGNAL(signalNotifyDevInfo(SGuiDev)), this, SLOT(updateDevInfo(SGuiDev)), Qt::QueuedConnection);

    BizStartNotifyDevInfo();//让 biz dev模块开始上传设备信息

    for (i=0; EM_NVR+i < EM_DEV_TYPE_MAX; ++i)
    {
        dev_type = EM_NVR+i;
        dev_ip_list.clear();

        if (BizGetDevIPList((EM_DEV_TYPE)dev_type, dev_ip_list))
        {
            ERR_PRINT("BizGetDevIPList failed, dev type:%d\n", dev_type);
            return;
        }

        switch (dev_type)
        {
        case EM_NVR:
            pmap = &map_nvr;
            break;

        case EM_PATROL_DEC:
            pmap = &map_patrol_dec;
            break;

        case EM_SWITCH_DEC:
            pmap = &map_switch_dec;
            break;

        default:
            ERR_PRINT("dev_type%d exception\n", dev_type);
            return;
        }

        for (iter = dev_ip_list.begin();
             iter != dev_ip_list.end();
             ++iter)
        {
            in.s_addr = *iter;

            //DBG_PRINT("dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));
            pdev = NULL;
            pdev = new SGuiDev;
            if (NULL == pdev)
            {
                ERR_PRINT("new SGuiDev failed, ip: %s\n", inet_ntoa(in));
                return;
            }

            if (BizGetDevInfo((EM_DEV_TYPE)dev_type, *iter, pdev))
            {
                ERR_PRINT("BizGetDevInfo failed, ip: %s\n", inet_ntoa(in));
                return;
            }

            if (!pmap->insert(std::make_pair(ntohl(*iter), pdev)).second)
            {
                ERR_PRINT("map insert failed, ip: %s\n", inet_ntoa(in));
                return;
            }
        }
    }

    //填充 tableWidget_dev
    dev_type = EM_NVR + ui->cmb_added_type->currentIndex();
    //DBG_PRINT("cmb_added_type: %d\n", dev_type);
    switch (dev_type)
    {
    case EM_NVR:
        pmap = &map_nvr;
        break;

    case EM_PATROL_DEC:
        pmap = &map_patrol_dec;
        break;

    case EM_SWITCH_DEC:
        pmap = &map_switch_dec;
        break;

    default:
        ERR_PRINT("dev_type%d cmb_added_type exception\n", dev_type);
        return;
    }

    for (row = 0, map_iter = pmap->begin();
         map_iter != pmap->end();
         ++row, ++map_iter)
    {
        //in.s_addr = ntohl(map_iter->first);
        //DBG_PRINT("addRowTableDev type: %d, ip: %s\n", map_iter->second->devicetype, inet_ntoa(in));
        addRowTableDev(row, map_iter->second);
    }
}

void page_dev_mgt::addRowTableSrh(int row, const SBiz_DeviceInfo_t *pdev)
{
    struct in_addr in;
    in.s_addr = pdev->deviceIP;

    ui->tableWidget_srh->insertRow(row);//添加一个空行，此时item(row, x)返回 NULL

    QTableWidgetItem *ptable_widget_item = new QTableWidgetItem;
    ptable_widget_item->setCheckState(Qt::Unchecked);
    ui->tableWidget_srh->setItem(row, 0, ptable_widget_item);
    ui->tableWidget_srh->setItem(row, 1, new QTableWidgetItem(strlist_devtype.at(pdev->devicetype-1)));
    ui->tableWidget_srh->setItem(row, 2, new QTableWidgetItem(QString(inet_ntoa(in))));
    ui->tableWidget_srh->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(pdev->maxChnNum)));
    //QTableWidgetItem *ptable_widget_item = new QTableWidgetItem(QString("%1").arg(iter->maxChnNum));
    //ptable_widget_item->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/msg_error.png"))));msg_info.png
    //ui->tableWidget_srh->setItem(row, 3, ptable_widget_item);
    //ui->tableWidget_srh->item(0, 3);
}

void page_dev_mgt::addRowTableDev(int row, const SGuiDev *pdev)
{
    struct in_addr in;
    in.s_addr = pdev->deviceIP;

    DBG_PRINT("row: %d\n", row);
    ui->tableWidget_dev->insertRow(row);//添加一个空行，此时item(row, x)返回 NULL

    QTableWidgetItem *ptable_widget_item = new QTableWidgetItem;
    ptable_widget_item->setCheckState(Qt::Unchecked);
    ui->tableWidget_dev->setItem(row, 0, ptable_widget_item);
    ui->tableWidget_dev->setItem(row, 1, new QTableWidgetItem(strlist_devtype.at(pdev->devicetype-1)));
    ui->tableWidget_dev->setItem(row, 2, new QTableWidgetItem(QString(inet_ntoa(in))));
    ui->tableWidget_dev->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(pdev->maxChnNum)));

    ptable_widget_item = new QTableWidgetItem;
    if (pdev->b_alive)//online
    {
        ptable_widget_item->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/msg_info.png"))));
    }
    else//offline
    {
        ptable_widget_item->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/msg_error.png"))));
    }
    ui->tableWidget_dev->setItem(row, 4, ptable_widget_item);
}

void page_dev_mgt::modifyRowTableDev(int row, const SGuiDev *pdev)
{
    struct in_addr in;
    QTableWidgetItem *ptable_widget_item = NULL;

    in.s_addr = pdev->deviceIP;
    QString ip_qstr(inet_ntoa(in));

    ptable_widget_item = ui->tableWidget_dev->item(row, 2);//ip
    if (NULL == ptable_widget_item)
    {
        ERR_PRINT("row: %d, col: %d, item == NULL\n", row, 2);
        return;
    }

    if (ip_qstr != ptable_widget_item->text())
    {
        ERR_PRINT("row: %d, dev ip(%s) != ptable_widget_item(%s)\n", \
                  row, ip_qstr.toUtf8().constData(), ptable_widget_item->text().toUtf8().constData());
        return;
    }

    ptable_widget_item = ui->tableWidget_dev->item(row, 3);//dev chn
    if (NULL == ptable_widget_item)
    {
        ERR_PRINT("row: %d, col: %d, item == NULL\n", row, 3);
        return;
    }
    ptable_widget_item->setText(QString("%1").arg(pdev->maxChnNum));

    ptable_widget_item = ui->tableWidget_dev->item(row, 4);//link status
    if (NULL == ptable_widget_item)
    {
        ERR_PRINT("row: %d, col: %d, item == NULL\n", row, 4);
        return;
    }

    if (pdev->b_alive)//online
    {
        ptable_widget_item->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/msg_info.png"))));
    }
    else//offline
    {
        ptable_widget_item->setIcon(QIcon(QPixmap(QString::fromUtf8(":/image/msg_error.png"))));
    }
}

int page_dev_mgt::addDev(SGuiDev *pdev)
{
    struct in_addr in;
    int ret = SUCCESS;
    MAP_IP_DEV *pmap = NULL;
    MAP_IP_DEV::iterator iter;
    int dev_type = pdev->devicetype;
    int row = 0;
    u32 ip_le = ntohl(pdev->deviceIP);
    in.s_addr = pdev->deviceIP;

    QMutexLocker locker(&mutex);

    ret = BizAddDev((EM_DEV_TYPE)dev_type, pdev->deviceIP);
    if (ret)
    {
        DBG_PRINT("BizAddDev failed, ret: %d, ip: %s\n", ret, inet_ntoa(in));
        return 0;
    }

    //add to MAP_IP_DEV
    switch (dev_type)
    {
    case EM_NVR:
        pmap = &map_nvr;
        break;

    case EM_PATROL_DEC:
        pmap = &map_patrol_dec;
        break;

    case EM_SWITCH_DEC:
        pmap = &map_switch_dec;
        break;

    default:
        ERR_PRINT("dev_type%d exception\n", dev_type);
        return -FAILURE;
    }

    //DBG_PRINT("currentIndex: %d\n", ui->cmb_added_type->currentIndex());
    //DBG_PRINT("devicetype: %d\n", pdev->devicetype);
    //add to tableWidget_dev
    if (ui->cmb_added_type->currentIndex() == pdev->devicetype-1)//当前正在显示该类型的设备，需要改变界面控件
    {
        //查找对应ip的设备行号
        for (row = 0, iter = pmap->begin(); iter != pmap->end(); ++row, ++iter)
        {
            if (ip_le == iter->first)
            {
                ERR_PRINT("tableWidget_dev exist, ip: %s\n", inet_ntoa(in));
                return -EIP_CONFLICT;
            }
            else if (ip_le < iter->first)//比较IP(le)
            {
                break;
            }
        }
        DBG_PRINT("add dev %s at row: %d\n", inet_ntoa(in), row);
        addRowTableDev(row, pdev);
    }

    if (!pmap->insert(std::make_pair(ip_le, pdev)).second)
    {
        ERR_PRINT("map insert failed, ip: %s\n", inet_ntoa(in));
        return -FAILURE;
    }

    return SUCCESS;
}

void page_dev_mgt::updateDevInfo(SGuiDev dev)
{
    QTableWidgetItem *ptable_widget_item = NULL;
    MAP_IP_DEV *pmap = NULL;
    MAP_IP_DEV::iterator iter;
    SGuiDev *pdev = NULL;
    int dev_type = dev.devicetype;
    u32 ip_le = ntohl(dev.deviceIP);
    int row = 0;
    struct in_addr in;

    in.s_addr = dev.deviceIP;
    DBG_PRINT("dev type: %d, ip: %s\n", dev_type, inet_ntoa(in));

    QMutexLocker locker(&mutex);

    switch (dev_type)
    {
    case EM_NVR:
        pmap = &map_nvr;
        break;

    case EM_PATROL_DEC:
        pmap = &map_patrol_dec;
        break;

    case EM_SWITCH_DEC:
        pmap = &map_switch_dec;
        break;

    default:
        ERR_PRINT("dev_type%d exception\n", dev_type);
        return;
    }

    //iter = pmap->find(ip_le);
    //if (iter != pmap->end())
    //查找对应ip的设备行号
    for (row = 0, iter = pmap->begin(); iter != pmap->end(); ++row, ++iter)
    {
        //ip le
        if (iter->first != ip_le)//确定排序后的行号，下面改变tableWidget_dev的index行中单元格
        {
            continue;
        }
        DBG_PRINT("find dev(%s) success, row: %d\n", inet_ntoa(in), row);

        if (iter->second)
        {
            pdev = iter->second;
        }
        else
        {
            ERR_PRINT("iter->second == NULL, ip: %s\n", inet_ntoa(in));
            continue;
        }

        if (pdev->devicetype != dev.devicetype)
        {
            ERR_PRINT("pdev->devicetype(%d) != dev.devicetype(%d)\n", pdev->devicetype, dev.devicetype);
            continue;
        }

        if (pdev->dev_idx == -1)//SGuiDev 构造时 dev_idx == -1
        {
            pdev->dev_idx = dev.dev_idx;
        }

        if (pdev->dev_idx != dev.dev_idx)
        {
            ERR_PRINT("pdev->dev_idx(%d) != dev.dev_idx(%d)\n", pdev->dev_idx, dev.dev_idx);
            continue;
        }

        pdev->maxChnNum = dev.maxChnNum;

        //if (pdev->b_alive != dev.b_alive)//在线状态发生改变
        {
            pdev->b_alive = dev.b_alive;

            if (ui->cmb_added_type->currentIndex() == pdev->devicetype-1)//当前正在显示该类型的设备，需要改变界面控件
            {
                modifyRowTableDev(row, pdev);
            }
        }
    }
}

void page_dev_mgt::on_btn_srh_clicked()
{
    VD_BOOL b_ret = FALSE;
    struct in_addr in;

    std::vector<SBiz_DeviceInfo_t> vnvr_list;
    std::vector<SBiz_DeviceInfo_t> vpatrol_dec_list;
    std::vector<SBiz_DeviceInfo_t> vswitch_dec_list;
    std::vector<SBiz_DeviceInfo_t> *pvdev_list = NULL;
    std::vector<SBiz_DeviceInfo_t>::iterator iter;
    //QTableWidgetItem *ptable_widget_item = NULL;

    int row = 0;
    int dev_type_idx = ui->cmb_srh_type->currentIndex();

    ui->tableWidget_srh->clearContents();//清除内容，释放内存，表头保留
    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->verticalHeader()->hide();
    qApp->processEvents();//刷新页面

    switch (dev_type_idx)
    {
        case 0: //NVR
            pvdev_list = &vnvr_list;
            break;
        case 1: //"轮巡型解码器"
            pvdev_list = &vpatrol_dec_list;
            break;
        case 2: //"切换型解码器"
            pvdev_list = &vswitch_dec_list;
            break;
        default:
            DBG_PRINT("dev type not support\n");
            return;
    }


    //::setShape(Qt::WaitCursor);
    //setCursor(QCursor(Qt::WaitCursor));
    b_ret = BizDevSearch(&vnvr_list, &vpatrol_dec_list, &vswitch_dec_list);
    //::setShape(Qt::ArrowCursor);
    //setCursor(QCursor(Qt::WaitCursor));
    if (!b_ret)
    {
        //ui->tableWidget_srh->setRowCount(pvdev_list->size());//行数，该类设备总数

        for (row = 0, iter = pvdev_list->begin();
             iter != pvdev_list->end();
             ++row, ++iter)
        {
            in.s_addr = iter->deviceIP;
            DBG_PRINT("devtype: %d(%s) row: %d, list deviceIP: %s\n",
                      iter->devicetype,
                      strlist_devtype.at(iter->devicetype-1).toUtf8().constData(),
                      row,
                      inet_ntoa(in));


            addRowTableSrh(row, &*iter);
        }
    }

    if (ui->tableWidget_srh->rowCount() != 0)
    {
        ui->tableWidget_srh->verticalHeader()->show();
        ui->tableWidget_srh->sortByColumn(2, Qt::AscendingOrder);
    }
}

void page_dev_mgt::on_btn_info_clicked()
{

}

void page_dev_mgt::on_btn_del_clicked()
{
    MAP_IP_DEV *pmap = NULL;
    MAP_IP_DEV::iterator iter;
    QString qstr;
    struct in_addr in;
    u32 dev_ip = 0;
    int dev_index = 0;
    int devicetype = 0;
    int row = 0;
    int ret = SUCCESS;
    SGuiDev *pdev = NULL;

    QMutexLocker locker(&mutex);

    for (row = 0; row < ui->tableWidget_dev->rowCount(); /* 空 */)
    {
        if (ui->tableWidget_dev->item(row, 0)->checkState())
        {
            DBG_PRINT("delete dev row: %d\n", row);
            //dev type str==>index
            qstr = ui->tableWidget_dev->item(row, 1)->text();
            dev_index = strlist_devtype.indexOf(qstr);
            if (dev_index < 0)
            {
                ERR_PRINT("dev type indexOf failed, row: %d\n", row);
                goto next_row;
            }
            devicetype = EM_NVR + dev_index;

            //dev ip
            qstr = ui->tableWidget_dev->item(row, 2)->text();
            dev_ip = inet_addr(qstr.toUtf8().constData());
            if (dev_ip == INADDR_NONE)
            {
                ERR_PRINT("inet_addr failed, row: %d\n", row);
                goto next_row;
            }
            in.s_addr = dev_ip;
            DBG_PRINT("delete dev %s row: %d\n", inet_ntoa(in), row);

            ret = BizDelDev(EM_DEV_TYPE(devicetype), dev_ip);
            if (ret)
            {
                ERR_PRINT("BizDelDev failed, ret: %d, ip: %s\n", ret, inet_ntoa(in));
                goto next_row;
            }

            //map delete
            switch (devicetype)
            {
            case EM_NVR:
                pmap = &map_nvr;
                break;

            case EM_PATROL_DEC:
                pmap = &map_patrol_dec;
                break;

            case EM_SWITCH_DEC:
                pmap = &map_switch_dec;
                break;

            default:
                ERR_PRINT("dev_type%d exception\n", devicetype);
                return;
            }

            iter = pmap->find(ntohl(dev_ip));
            if (iter == pmap->end())
            {
                ERR_PRINT("map not found IP(%s) failed, dev type: %d\n", inet_ntoa(in), devicetype);
                goto next_row;
            }

            pdev = iter->second;
            delete pdev;
            pdev = NULL;

            pmap->erase(iter);

            //ui delete
            ui->tableWidget_dev->removeRow(row);
            DBG_PRINT("delete dev success, ip: %s, dev type: %d\n", inet_ntoa(in), devicetype);
            continue;
        }

next_row:
        ++row;
    }
}

void page_dev_mgt::on_btn_add_clicked()
{
    int i=0;
    int add_row = 0;
    int srh_row = ui->tableWidget_srh->rowCount();
    SGuiDev *pdev = NULL;
    QString qstr;
    int dev_index = 0;
    bool ok = true;
    int maxChnNum = 0;
    struct in_addr in;
    int ret = 0;

    for (i=0; i<srh_row; ++i)//扫描搜索表格所有行
    {
        if (ui->tableWidget_srh->item(i, 0)->checkState())
        {
            pdev = NULL;
            pdev = new SGuiDev;
            if (NULL == pdev)
            {
                ERR_PRINT("new SGuiDev failed, ip: %s\n", inet_ntoa(in));
                continue;
            }

            //dev type str==>index
            qstr = ui->tableWidget_srh->item(i, 1)->text();
            dev_index = strlist_devtype.indexOf(qstr);
            if (dev_index < 0)
            {
                delete pdev;
                pdev = NULL;

                ERR_PRINT("dev type indexOf failed, row: %d\n", i);
                continue;
            }
            pdev->devicetype = EM_NVR + dev_index;

            //dev ip
            qstr = ui->tableWidget_srh->item(i, 2)->text();
            pdev->deviceIP = inet_addr(qstr.toUtf8().constData());
            if (pdev->deviceIP == INADDR_NONE)
            {
                delete pdev;
                pdev = NULL;

                ERR_PRINT("inet_addr failed, row: %d\n", i);
                continue;
            }
            in.s_addr = pdev->deviceIP;
            DBG_PRINT("add dev %s row: %d\n", inet_ntoa(in), i);

            //dev chnnum
            qstr = ui->tableWidget_srh->item(i, 3)->text();
            maxChnNum = qstr.toInt(&ok);
            if (!ok)
            {
                delete pdev;
                pdev = NULL;

                ERR_PRINT("row: %d, chn num toInt failed\n", i);
                continue;
            }
            pdev->maxChnNum = maxChnNum;

            ret = addDev(pdev);
            if (ret)
            {
                delete pdev;
                pdev = NULL;

                DBG_PRINT("addDev failed, ret: %d\n", ret);
            }
        }
    }
    //ui->tableWidget_dev->sortByColumn(2, Qt::AscendingOrder);
}

void page_dev_mgt::cmbSrhChange(int index)
{
    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->clearContents();
}

void page_dev_mgt::cmbAddChange(int index)
{

}

void page_dev_mgt::tableWidgetSrhDBClicked(int row, int column)
{
    //ui->tableWidget_srh->setCurrentCell(row, column, QItemSelectionModel::Select);//设置该行为选中
    ui->tableWidget_srh->selectRow(row);
    DBG_PRINT("DoubleClicked row: %d, col: %d\n", row, column);
}



void page_dev_mgt::showEvent( QShowEvent * event )
{
    ui->tableWidget_srh->setRowCount(0);
    ui->tableWidget_srh->clearContents();
}




