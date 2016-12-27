#include <time.h>

#include <QtGui>
#include "bond.h"

#include "form_playback.h"
#include "ui_form_playback.h"
#include "frmmessagebox.h"

#include "page_manager.h"
#include "page_main.h"
#include "page_dev_mgt.h"

#include "biz_system_complex.h"
#include "biz_playback.h"

#define MAX_FILE_NUMS (10)
#define MB (1024*1024)

form_playback::form_playback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_playback),
    search_nvr_ip(INADDR_NONE),
    search_nvr_chn(INADDR_NONE)//只是使用该无效值，没有其他意思
{
    memset(&search_para, 0, sizeof(ifly_recsearch_param_t));

    memset(&search_result.result_desc, 0, sizeof(ifly_search_desc_t));
    search_result.pfile_info = new ifly_recfileinfo_t[MAX_FILE_NUMS];
    if (NULL == search_result.pfile_info)
    {
        ERR_PRINT("NULL == pfile_info");
    }

    ui->setupUi(this);
    init_style();
    init_form();//控件
}

form_playback::~form_playback()
{
    delete ui;
    if (search_result.pfile_info)
    {
        delete[] search_result.pfile_info;

        search_result.pfile_info = NULL;
    }
}

//初始化无边框窗体
void form_playback::init_style()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void form_playback::init_form() //控件
{
    setupWidgetLeft();
    setupWidgetBottom();

	connect(gp_bond, SIGNAL(signalNotifyPlaybackInfo(SPlaybackNotify_t)), this, SLOT(slotNotifyPlaybackInfo(SPlaybackNotify_t)), Qt::QueuedConnection);
}

void form_playback::setupWidgetLeft()
{
    //nvr treeWidget
    setupTreeWidgetNvr();

    //search date and time
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setProperty("noinput", true);

    SDateTime dt;
    memset(&dt, 0, sizeof(SDateTime));

    int ret = BizGetTime(&dt);
    if (ret)
    {
        ERR_PRINT("BizGetTime failed, ret: %d\n", ret);
        return ;
    }

    QDate qd(dt.nYear, dt.nMonth, dt.nDay);
    ui->dateEdit->setDate(qd);

    QTime qt_start(0, 0, 0);// 00:00:00
    ui->timeEdit_start->setTime(qt_start);

    QTime qt_end(23, 59, 59);// 23:59:59
    ui->timeEdit_end->setTime(qt_end);
}

void form_playback::setupTreeWidgetNvr()
{
    QMutexLocker locker(&mutex);
    //dev list
    page_dev_mgt *page_dev = (page_dev_mgt *)getPage(PAGE_DEV_MGT);
    if (NULL == page_dev)
    {
        ERR_PRINT("getPage(PAGE_DEV_MGT) failed\n");
        return;
    }
    connect(page_dev, SIGNAL(signalDevInfoChange(SGuiDev)), this, SLOT(refreshDevInfo(SGuiDev)));//更新设备 alive 状态

    QStringList strlist_devtype;
    if (page_dev->getDevTypeStrList(strlist_devtype))
    {
        ERR_PRINT("getDevTypeStrList failed\n");
        return;
    }

    if (strlist_devtype.size() < EM_DEV_TYPE_MAX-EM_NVR)
    {
        ERR_PRINT("strlist_devtype size(%d) < EM_DEV_TYPE_MAX(%d)-EM_NVR(%d)\n",
                  strlist_devtype.size(), EM_DEV_TYPE_MAX, EM_NVR);
        return ;
    }

    ui->treeWidget_nvr->setColumnCount(1);//1列
    ui->treeWidget_nvr->setHeaderLabels(QStringList() << strlist_devtype.at(EM_NVR-EM_NVR));//EM_NVR-EM_NVR

    //get dev info
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

            //add chn item
            list_chn.clear();
            for (i = 0; i < dev.maxChnNum; ++i)
            {
                chn_item = NULL;
                chn_item = new QTreeWidgetItem;
                if (NULL == chn_item)
                {
                    ERR_PRINT("chn new QTreeWidgetItem failed\n");
                    break ;
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
        }
        else
        {
            dev_item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }

        list_dev.append(dev_item);
    }

    ui->treeWidget_nvr->addTopLevelItems(list_dev);
}

void form_playback::setupWidgetBottom()
{
    //播放控制
    //btn play/pause
    ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_play.bmp")));
    ui->btn_play->setToolTip(QString::fromUtf8("播放/暂停"));

    //btn stop
    ui->btn_stop->setToolTip(QString::fromUtf8("停止"));

    //btn frame
    ui->btn_next_frame->setToolTip(QString::fromUtf8("单帧前进"));

    ui->slider_play->setToolTip(QString::fromUtf8("播放速率"));

    //搜索文件 控制
    ui->btn_page_start->setToolTip(QString::fromUtf8("首页"));
    ui->btn_page_pre->setToolTip(QString::fromUtf8("上一页"));
    ui->btn_page_next->setToolTip(QString::fromUtf8("下一页"));
    ui->btn_page_end->setToolTip(QString::fromUtf8("尾页"));

    ui->lab_page->setToolTip(QString::fromUtf8("当前页/总页数"));
    ui->lab_page->setText(QString::fromUtf8("0/0"));
    ui->lab_page->adjustSize();

    //btn extra
    ui->btn_extra->setIcon(QIcon(QString::fromUtf8(":/image/up.bmp")));
    ui->btn_extra->setToolTip(QString::fromUtf8("显示文件列表"));

    ui->btn_extra->setCheckable(true);
    ui->btn_extra->setAutoDefault(false);

    setupTableWidgetResult();
    ui->widget_result->hide();

    connect(ui->btn_extra, SIGNAL(toggled(bool)), this, SLOT(showTableWidget(bool)));
    connect(ui->tableWidget_left, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tableWidgetClicked(QTableWidgetItem*)));
    connect(ui->tableWidget_left, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(tableWidgetDoubleClicked(QTableWidgetItem*)));
    connect(ui->tableWidget_right, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(tableWidgetClicked(QTableWidgetItem*)));
    connect(ui->tableWidget_right, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(tableWidgetDoubleClicked(QTableWidgetItem*)));
}

void form_playback::setupTableWidgetResult()
{
    QStringList header;
    header.append(QString::fromUtf8("序号"));
    header.append(QString::fromUtf8("起始时间"));
    header.append(QString::fromUtf8("结束时间"));
    header.append(QString::fromUtf8("文件大小"));

    ui->tableWidget_left->verticalHeader()->setVisible(false);//列表头不可见
    //ui->tableWidget_left->horizontalHeader()->setVisible(false);//行表头不可见
    ui->tableWidget_left->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_left->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_left->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_left->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_left->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_left->horizontalHeader()->setStretchLastSection(true);//最后一列单元格占满 tablewidget
    ui->tableWidget_left->setRowCount(5);
    ui->tableWidget_left->clearContents();
    ui->tableWidget_left->setColumnCount(4);
    ui->tableWidget_left->setColumnWidth(0,50);
    ui->tableWidget_left->setColumnWidth(1,100);
    ui->tableWidget_left->setColumnWidth(2,100);
    //设置表头
    QTableWidgetItem *__qtablewidgetitem0 = new QTableWidgetItem();
    ui->tableWidget_left->setHorizontalHeaderItem(0, __qtablewidgetitem0);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    ui->tableWidget_left->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    ui->tableWidget_left->setHorizontalHeaderItem(2, __qtablewidgetitem2);
    QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
    ui->tableWidget_left->setHorizontalHeaderItem(3, __qtablewidgetitem3);
    ui->tableWidget_left->setHorizontalHeaderLabels(header);




    ui->tableWidget_right->verticalHeader()->setVisible(false);//列表头不可见
    //ui->tableWidget_right->horizontalHeader()->setVisible(false);//行表头不可见
    ui->tableWidget_right->setFocusPolicy(Qt::NoFocus);//让table失去焦点，防止没有选中行时，添加第一行
    ui->tableWidget_right->setSelectionBehavior(QAbstractItemView::SelectRows);//点击选择整行
    ui->tableWidget_right->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_right->setAlternatingRowColors(true);//奇偶行不同颜色显示
    ui->tableWidget_right->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    ui->tableWidget_right->horizontalHeader()->setStretchLastSection(true);//最后一列单元格占满 tablewidget
    ui->tableWidget_right->setRowCount(5);
    ui->tableWidget_right->clearContents();
    ui->tableWidget_right->setColumnCount(4);
    ui->tableWidget_right->setColumnWidth(0,50);
    ui->tableWidget_right->setColumnWidth(1,100);
    ui->tableWidget_right->setColumnWidth(2,100);
    //设置表头
    __qtablewidgetitem0 = new QTableWidgetItem();
    ui->tableWidget_right->setHorizontalHeaderItem(0, __qtablewidgetitem0);
    __qtablewidgetitem1 = new QTableWidgetItem();
    ui->tableWidget_right->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    __qtablewidgetitem2 = new QTableWidgetItem();
    ui->tableWidget_right->setHorizontalHeaderItem(2, __qtablewidgetitem2);
    __qtablewidgetitem3 = new QTableWidgetItem();
    ui->tableWidget_right->setHorizontalHeaderItem(3, __qtablewidgetitem3);
    ui->tableWidget_right->setHorizontalHeaderLabels(header);
}

void form_playback::tableWidgetClicked(QTableWidgetItem * item)
{
    //ui->tableWidget_left->clearSelection()清除选择区域
    if (NULL == item)
    {
        ERR_PRINT("NULL == item\n");

        return ;
    }

    QTableWidget *ptable_widget = NULL;
    ptable_widget = item->tableWidget();
    if (NULL == ptable_widget)
    {
        ERR_PRINT("NULL == ptable_widget\n");
        return ;
    }

    if (ui->tableWidget_left == ptable_widget)
    {
        ui->tableWidget_right->clearSelection();//清除选择区域
    }
    else if (ui->tableWidget_right == ptable_widget)
    {
        ui->tableWidget_left->clearSelection();
    }
    else
    {
        ERR_PRINT("ptable_widget neither left nor right\n");
        return ;
    }
}

void form_playback::tableWidgetDoubleClicked(QTableWidgetItem * item)
{
    if (NULL == item)
    {
        ERR_PRINT("NULL == item\n");

        return ;
    }

    QTableWidget *ptable_widget = NULL;
    ptable_widget = item->tableWidget();
    if (NULL == ptable_widget)
    {
        ERR_PRINT("NULL == ptable_widget\n");
        return ;
    }

    if (ui->tableWidget_left != ptable_widget
            && ui->tableWidget_right != ptable_widget)
    {
        ERR_PRINT("ptable_widget neither left nor right\n");
        return ;
    }

    int file_idx = item->row();//file 在search_result.pfile_info 数组中的index，并非startID之类
    if (ui->tableWidget_right == ptable_widget)
    {
        file_idx += MAX_FILE_NUMS / 2;
    }
    DBG_PRINT("file_no: %d\n", file_idx);

    if (file_idx < 0
            || file_idx >= search_result.result_desc.endID-search_result.result_desc.startID+1)
    {
        ERR_PRINT("file_idx: %d invalid, startID: %d, endID: %d\n",
                  file_idx, search_result.result_desc.startID, search_result.result_desc.endID);
        return ;
    }

    DBG_PRINT("file name: %s\n", search_result.pfile_info[file_idx].filename);
    int ret = SUCCESS;
    ret = BizPlaybackStartByFile(search_nvr_ip, &search_result.pfile_info[file_idx]);
    if (ret)
    {
        ERR_PRINT("BizPlaybackStartByFile failed, ret: %d\n", ret);
    }
}

void form_playback::showTableWidget(bool b)
{
    ui->widget_result->setVisible(b);

    DBG_PRINT("widget_play_ctrl height: %d\n", ui->widget_play_ctrl->height());

    QIcon icon;
    QString icon_name;
    QString tooltip;
    if (b)
    {
        icon_name = QString::fromUtf8(":/image/down.bmp");
        tooltip = QString::fromUtf8("隐藏文件列表");
    }
    else
    {
        icon_name = QString::fromUtf8(":/image/up.bmp");
        tooltip = QString::fromUtf8("显示文件列表");
    }
    icon.addFile(icon_name, QSize(), QIcon::Normal, QIcon::Off);
    ui->btn_extra->setIcon(icon);
    ui->btn_extra->setToolTip(tooltip);
}

void form_playback::refreshDevInfo(SGuiDev dev)
{
    QMutexLocker locker(&mutex);
    struct in_addr in;
    QString qstr_ip;
    QList<QTreeWidgetItem *> list_dev_find;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }
    DBG_PRINT("dev type: %d, ip: %s\n", dev.devicetype, qstr_ip.toUtf8().constData());

    if (dev.devicetype != EM_NVR)
    {
        DBG_PRINT("dev.devicetype(%d) != EM_NVR", dev.devicetype);
        return ;
    }

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

void form_playback::slotNotifyPlaybackInfo(SPlaybackNotify_t playback_msg)123
{
	//0 ��������1 �������
	if (0 == playback_msg.msg_type)
	{
		
	}
	else if (1 == playback_msg.msg_type)
	{
		
	}		
}


void form_playback::refreshWidgetResult()
{
    if (0 == search_result.result_desc.startID)
    {
        ShowMessageBoxInfo(QString::fromUtf8("没有搜索到满足条件的文件"));

        return ;
    }

    u32 page_total = (search_result.result_desc.sum + MAX_FILE_NUMS -1) / MAX_FILE_NUMS;
    u32 page_cur = search_result.result_desc.startID / MAX_FILE_NUMS + 1;
    DBG_PRINT("page_total: %d, page_cur: %d\n", page_total, page_cur);

    ui->lab_page->setText(QString::fromUtf8("%1/%2").arg(page_cur).arg(page_total));

    refreshWidgetResultLeft();
    refreshWidgetResultright();
}

void form_playback::refreshWidgetResultLeft()
{
    ui->tableWidget_left->clearContents();

    u32 start_time = 0;
    u32 end_time = 0;
    struct tm tm_time;
    QTableWidgetItem *item = NULL;
    QString str;
    QTime qtime;
    u32 result_file_nums = search_result.result_desc.endID - search_result.result_desc.startID + 1;
    int show_rows = MIN(MAX_FILE_NUMS/2, result_file_nums);
    DBG_PRINT("show_rows: %d\n", show_rows);

    for (int i=0; i<show_rows; ++i)
    {
        start_time = search_result.pfile_info[i].start_time;
        end_time = search_result.pfile_info[i].end_time;
        start_time += 8*3600;//时区偏移
        end_time += 8*3600;

        //序号
        str = QString(QString::fromUtf8("%1").arg(i+search_result.result_desc.startID));
        item = new QTableWidgetItem();
        item->setText(str);
        ui->tableWidget_left->setItem(i, 0, item);

        //起始时间
        gmtime_r((time_t *)&start_time, &tm_time);
        qtime = QTime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        item = new QTableWidgetItem();
        item->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
        ui->tableWidget_left->setItem(i, 1, item);

        //结束时间
        gmtime_r((time_t *)&end_time, &tm_time);
        qtime = QTime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        item = new QTableWidgetItem();
        item->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
        ui->tableWidget_left->setItem(i, 2, item);

        //文件大小
        str = QString(QString::fromUtf8("%1MB").arg(search_result.pfile_info[i].size / MB));
        item = new QTableWidgetItem();
        item->setText(str);
        ui->tableWidget_left->setItem(i, 3, item);
    }
}

void form_playback::refreshWidgetResultright()
{
    ui->tableWidget_right->clearContents();

    u32 start_time = 0;
    u32 end_time = 0;
    struct tm tm_time;
    QTableWidgetItem *item = NULL;
    QString str;
    QTime qtime;
    u32 result_file_nums = search_result.result_desc.endID - search_result.result_desc.startID + 1;

    if (result_file_nums <= MAX_FILE_NUMS/2)//tableWidget_left中已经足够显示
    {
        return ;
    }

    int show_rows = MIN(MAX_FILE_NUMS/2, result_file_nums - MAX_FILE_NUMS/2);

    DBG_PRINT("show_rows: %d\n", show_rows);

    for (int i=0; i<show_rows; ++i)
    {
        start_time = search_result.pfile_info[i+MAX_FILE_NUMS/2].start_time;
        end_time = search_result.pfile_info[i+MAX_FILE_NUMS/2].end_time;
        start_time += 8*3600;//时区偏移
        end_time += 8*3600;

        //序号
        str = QString(QString::fromUtf8("%1").arg(i+MAX_FILE_NUMS/2+search_result.result_desc.startID));
        item = new QTableWidgetItem();
        item->setText(str);
        ui->tableWidget_right->setItem(i, 0, item);

        //起始时间
        gmtime_r((time_t *)&start_time, &tm_time);
        qtime = QTime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        item = new QTableWidgetItem();
        item->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
        ui->tableWidget_right->setItem(i, 1, item);

        //结束时间
        gmtime_r((time_t *)&end_time, &tm_time);
        qtime = QTime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        item = new QTableWidgetItem();
        item->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
        ui->tableWidget_right->setItem(i, 2, item);

        //文件大小
        str = QString(QString::fromUtf8("%1MB").arg(search_result.pfile_info[i+MAX_FILE_NUMS/2].size / MB));
        item = new QTableWidgetItem();
        item->setText(str);
        ui->tableWidget_right->setItem(i, 3, item);
    }
}

//得到当前选中的NVR IP和通道号
int form_playback::getCurrentNvrChn(u32 *pnvr_ip, u32 *pnvr_chn)
{
    QString str_nvr_ip;
    QString str_chn;
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;

    QTreeWidgetItem *item = ui->treeWidget_nvr->currentItem();
    if (NULL == item)
    {
        ERR_PRINT("current item == NULL\n");

        ShowMessageBoxInfo(QString::fromUtf8("请选定NVR的一个通道！"));
        return -FAILURE;
    }

    if (NULL == item->parent())
    {
        ERR_PRINT("current item parent == NULL\n");

        ShowMessageBoxInfo(QString::fromUtf8("请选定NVR的一个通道！"));
        return -FAILURE;
    }

    str_nvr_ip = item->parent()->text(0);
    nvr_ip = inet_addr(str_nvr_ip.toUtf8().constData());
    if (INADDR_NONE == nvr_ip)
    {
        ERR_PRINT("nvr ip invalid, str_nvr_ip: %s\n", str_nvr_ip.toUtf8().constData());

        ShowMessageBoxError(QString::fromUtf8("选定的NVR IP地址无效！"));
        return -FAILURE;
    }

    //req nvr chn
    str_chn = item->text(0);
    if (!str_chn.contains(QString::fromUtf8("通道"), Qt::CaseInsensitive))//不含有"通道"
    {
        ERR_PRINT("chn str: %s, contains invalid\n", str_chn.toUtf8().constData());

        ShowMessageBoxError(QString::fromUtf8("选定的NVR通道有误！"));
        return -FAILURE;
    }

    nvr_chn = item->parent()->indexOfChild(item);
    DBG_PRINT("nvr_chn: %d\n", nvr_chn);

    *pnvr_ip = nvr_ip;
    *pnvr_chn = nvr_chn;

    return SUCCESS;
}

void form_playback::on_btn_srh_clicked()
{
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;
    u32 start_time = 0;
    u32 end_time = 0;
    struct tm tmtemp;
    QDate qd;
    QTime qt;
    int ret = SUCCESS;

    memset(&search_para, 0, sizeof(ifly_recsearch_param_t));

    //得到当前选中的NVR IP和通道号
    ret = getCurrentNvrChn(&nvr_ip, &nvr_chn);
    if (ret)
    {
        return ;
    }
    search_nvr_ip = nvr_ip;
    search_nvr_chn = nvr_chn;

    //date & time
    qd = ui->dateEdit->date();

    tmtemp.tm_year = qd.year() -1900;
    tmtemp.tm_mon = qd.month() -1;
    tmtemp.tm_mday = qd.day();

    qt = ui->timeEdit_start->time();
    tmtemp.tm_hour = qt.hour();
    tmtemp.tm_min = qt.minute();
    tmtemp.tm_sec = qt.second();
    start_time = mktime(&tmtemp);
    start_time -= 8*3600; //时区偏移

    qt = ui->timeEdit_end->time();
    tmtemp.tm_hour = qt.hour();
    tmtemp.tm_min = qt.minute();
    tmtemp.tm_sec = qt.second();
    end_time = mktime(&tmtemp);
    end_time -= 8*3600; //时区偏移

#if 0
//录像文件类型
enum NETDVR_REC_INDEX_MASK
{
    NETDVR_REC_INDEX_TIMER = 0x1,
    NETDVR_REC_INDEX_MD = 0x2,
    NETDVR_REC_INDEX_ALARM = 0x4,
    NETDVR_REC_INDEX_HAND = 0x8,
    NETDVR_REC_INDEX_ALL = 0x10,
};
#endif
    search_para.channel_mask |= 1<<nvr_chn;
    search_para.type_mask   = 0x10;
    search_para.start_time  = start_time;
    search_para.end_time    = end_time;
    search_para.startID     = 1;  //must >= 1
    search_para.max_return  = MAX_FILE_NUMS; //must <= 24

    ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
        return ;
    }

#if 0
    if (search_result.result_desc.startID < search_result.result_desc.endID)
    {
        start_time = search_result.pfile_info[0].start_time;
        end_time = search_result.pfile_info[0].end_time;

        start_time += 8*3600;
        end_time += 8*3600;

        struct tm tm_time;
        gmtime_r((time_t *)&start_time, &tm_time);

        printf("tm_year: %d\n", tm_time.tm_year + 1900);
        printf("tm_mon: %d\n", tm_time.tm_mon + 1);
        printf("tm_mday: %d\n", tm_time.tm_mday);
        printf("tm_hour: %d\n", tm_time.tm_hour);
        printf("tm_min: %d\n", tm_time.tm_min);
        printf("tm_sec: %d\n", tm_time.tm_sec);
    }
#endif

    refreshWidgetResult();
}

void form_playback::on_btn_page_start_clicked()
{
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;
    int ret = SUCCESS;

    ret = getCurrentNvrChn(&nvr_ip, &nvr_chn);
    if (ret)
    {
        return ;
    }

    if (nvr_ip != search_nvr_ip
            || nvr_chn != search_nvr_chn)
    {
        DBG_PRINT("nvr or chn changed, research it\n");
        ShowMessageBoxInfo(QString::fromUtf8("NVR通道发生改变，请选定后重新搜索！"));
        return ;
    }

    if (search_result.result_desc.startID == 1)//当前就在起始页
    {
        DBG_PRINT("current page start\n");
        return ;
    }

    search_para.startID = 1;
    ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
        return ;
    }

    refreshWidgetResult();
}

void form_playback::on_btn_page_pre_clicked()
{
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;
    int ret = SUCCESS;

    ret = getCurrentNvrChn(&nvr_ip, &nvr_chn);
    if (ret)
    {
        return ;
    }

    if (nvr_ip != search_nvr_ip
            || nvr_chn != search_nvr_chn)
    {
        DBG_PRINT("nvr or chn changed, research it\n");
        ShowMessageBoxInfo(QString::fromUtf8("NVR通道发生改变，请选定后重新搜索！"));
        return ;
    }

    if (search_result.result_desc.startID == 1)//当前就在起始页
    {
        DBG_PRINT("current page start\n");
        return ;
    }

    u32 page_total = (search_result.result_desc.sum + MAX_FILE_NUMS -1) / MAX_FILE_NUMS;
    u32 page_cur = search_result.result_desc.startID / MAX_FILE_NUMS + 1;
    DBG_PRINT("page_total: %d, page_cur: %d\n", page_total, page_cur);

    search_para.startID -= MAX_FILE_NUMS;
    ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
        return ;
    }

    refreshWidgetResult();
}

void form_playback::on_btn_page_next_clicked()
{
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;
    int ret = SUCCESS;

    ret = getCurrentNvrChn(&nvr_ip, &nvr_chn);
    if (ret)
    {
        return ;
    }

    if (nvr_ip != search_nvr_ip
            || nvr_chn != search_nvr_chn)
    {
        DBG_PRINT("nvr or chn changed, research it\n");
        ShowMessageBoxInfo(QString::fromUtf8("NVR通道发生改变，请选定后重新搜索！"));
        return ;
    }

    if (search_result.result_desc.endID == search_result.result_desc.sum)//当前就在末尾页
    {
        DBG_PRINT("current page end\n");
        return ;
    }

    u32 page_total = (search_result.result_desc.sum + MAX_FILE_NUMS -1) / MAX_FILE_NUMS;
    u32 page_cur = search_result.result_desc.startID / MAX_FILE_NUMS + 1;
    DBG_PRINT("page_total: %d, page_cur: %d\n", page_total, page_cur);

    search_para.startID += MAX_FILE_NUMS;
    ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
        return ;
    }

    refreshWidgetResult();
}

void form_playback::on_btn_page_end_clicked()
{
    u32 nvr_ip = INADDR_NONE;
    u32 nvr_chn = INADDR_NONE;
    int ret = SUCCESS;

    ret = getCurrentNvrChn(&nvr_ip, &nvr_chn);
    if (ret)
    {
        return ;
    }

    if (nvr_ip != search_nvr_ip
            || nvr_chn != search_nvr_chn)
    {
        DBG_PRINT("nvr or chn changed, research it\n");
        ShowMessageBoxInfo(QString::fromUtf8("NVR通道发生改变，请选定后重新搜索！"));
        return ;
    }

    if (search_result.result_desc.endID == search_result.result_desc.sum)//当前就在末尾页
    {
        DBG_PRINT("current page end\n");
        return ;
    }

    u32 page_total = (search_result.result_desc.sum + MAX_FILE_NUMS -1) / MAX_FILE_NUMS;
    u32 page_cur = search_result.result_desc.startID / MAX_FILE_NUMS + 1;
    DBG_PRINT("page_total: %d, page_cur: %d\n", page_total, page_cur);

    search_para.startID = (page_total-1)*MAX_FILE_NUMS + 1;
    ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
        return ;
    }

    refreshWidgetResult();
}

void form_playback::on_btn_to_dec_clicked()
{
    ShowMessageBoxInfo(QString::fromUtf8("该功能尚未实现！"));

    return ;
}

//play ctl
void form_playback::on_btn_stop_clicked()
{
    int ret = SUCCESS;

    ret = BizPlaybackStop();
    if (ret)
    {
        ERR_PRINT("BizPlaybackStartByFile failed, ret: %d\n", ret);
    }
}

void form_playback::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        this->close();

        page_main* pmain = (page_main*)getPage(PAGE_MAIN);
        if (NULL == pmain)
        {
            ERR_PRINT("getPage(PAGE_MAIN) failed\n");
            return;
        }
        pmain->show();
    }
}




