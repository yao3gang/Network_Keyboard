#include <time.h>

#include <QtGui>
#include "bond.h"

#include "form_playback.h"
#include "ui_form_playback.h"
#include "frmmessagebox.h"
#include "dialogprogress.h"

#include "page_manager.h"
#include "page_main.h"
#include "page_dev_mgt.h"

#include "biz_system_complex.h"
#include "biz_4_gui.h"

#define MAX_FILE_NUMS (10)
#define MB (1024*1024)

form_playback::form_playback(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::form_playback)
    , m_displayLabel(NULL)
    , search_nvr_ip(INADDR_NONE)
    , search_nvr_chn(INVALID_VALUE)//只是使用该无效值，没有其他意思
    , play_nvr_ip(INADDR_NONE)//当前播放相关数据
    , play_nvr_chn(INVALID_VALUE)
    , play_status(EM_PLAY_STATUS_STOP)
    , b_slider_mover(false)
    , slider_pressed_pos(0)
    , play_pos(0)
    , play_rate_index(3)
    , file_down_index(INVALID_VALUE)
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

    //btn step
    ui->btn_step->setToolTip(QString::fromUtf8("单帧前进"));

    //slider
    QTime qtime(0, 0, 0);
    ui->lab_time_start->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
    ui->lab_time_end->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));

    ui->slider_play->setMinimum(0);
    ui->slider_play->setMaximum(0);
    ui->slider_play->setSingleStep(1);

    //slider label 显示鼠标拖动游标时的当前时间
    m_displayLabel=new QLabel(this);
    //m_displayLabel->setFixedSize(QSize(20,20));
    //设置游标背景为白色
    m_displayLabel->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    m_displayLabel->setPalette(palette);

    m_displayLabel->setAlignment(Qt::AlignCenter);

    m_displayLabel->setVisible(false);
    //m_displayLabel->move(ui->slider_play->x(), ui->slider_play->y()+3);

    //rate
    ui->cmb_rate->addItem(QString::fromUtf8("1/8倍速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("1/4倍速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("1/2倍速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("正常速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("2倍速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("4倍速率"));
    ui->cmb_rate->addItem(QString::fromUtf8("8倍速率"));
    ui->cmb_rate->setCurrentIndex(3);//正常速率
    play_rate_index = 3;

    ui->cmb_rate->setToolTip(QString::fromUtf8("播放速率"));
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

    connect(ui->cmb_rate, SIGNAL(currentIndexChanged(int)), this, SLOT(set_play_rate(int)));
    connect(ui->slider_play, SIGNAL(costomSliderPressed(int)), this, SLOT(slider_pressed(int)));
    connect(ui->slider_play, SIGNAL(sliderMoved(int)), this, SLOT(slider_moved(int)));
    connect(ui->slider_play, SIGNAL(costomSliderReleased(int)), this, SLOT(slider_released(int)));

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

    int row_clicked = item->row();
    DBG_PRINT("row_clicked: %d\n", row_clicked);
    //file_down_index

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
        file_down_index = row_clicked;
    }
    else if (ui->tableWidget_right == ptable_widget)
    {
        ui->tableWidget_left->clearSelection();
        file_down_index = row_clicked + MAX_FILE_NUMS/2;
    }
    else
    {
        ERR_PRINT("ptable_widget neither left nor right\n");
        file_down_index = INVALID_VALUE;
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

    if (INADDR_NONE == search_nvr_ip)
    {
        ERR_PRINT("INADDR_NONE == search_nvr_ip\n");
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
    ret = BizPlaybackStartByFile(0, search_nvr_ip, &search_result.pfile_info[file_idx]);
    if (ret)
    {
        ERR_PRINT("BizPlaybackStartByFile failed, ret: %d\n", ret);
    }

    play_nvr_ip = search_nvr_ip;
    play_nvr_chn = search_nvr_chn;
    play_file = search_result.pfile_info[file_idx];
    play_pos = 0;
    play_rate_index = 3;
    play_status = EM_PLAY_STATUS_RUNNING;
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

void form_playback::slotNotifyPlaybackInfo(SPlaybackNotify_t playback_msg)
{
    EMBIZEVENT type = playback_msg.type;
    u32 playback_chn = playback_msg.playback_chn;
    DBG_PRINT("type: %d, chn: %d\n", type, playback_chn);

    switch (type)
    {
        case EM_BIZ_EVENT_PLAYBACK_INIT:
        {

        } break;

        case EM_BIZ_EVENT_PLAYBACK_START:
        {
            if (playback_chn < 0x10)//回放
            {
                u32 start_time = 0;
                u32 end_time = 0;
                struct tm tm_time;

                DBG_PRINT("play_status(%d)\n", play_status);

                //time: start & end
                start_time = play_file.start_time;
                end_time = play_file.end_time;

                if (start_time >= end_time)
                {
                    ERR_PRINT("start_time >= end_time\n");

                    return;
                }

                start_time += 8*3600;
                end_time += 8*3600;

                gmtime_r((time_t *)&start_time, &tm_time);
                QTime qtime_start(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
                ui->lab_time_start->setText(qtime_start.toString(QString::fromUtf8("HH:mm:ss")));//play_start_time

                gmtime_r((time_t *)&end_time, &tm_time);
                QTime qtime_end(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
                ui->lab_time_end->setText(qtime_start.toString(QString::fromUtf8("HH:mm:ss"))
                                          + QString::fromUtf8("/")
                                          + qtime_end.toString(QString::fromUtf8("HH:mm:ss")));//play_cur_time/play_end_time
                ui->lab_time_end->adjustSize();

                //slider
                ui->slider_play->setMinimum(0);
                ui->slider_play->setMaximum(end_time - start_time);

                play_pos = 0;
                play_status = EM_PLAY_STATUS_RUNNING;

                //ui
                //播放控制
                //btn play/pause
                ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_play.bmp")));

                DBG_PRINT("start_time: %s, end_time: %s\n", ui->lab_time_start->text().toUtf8().constData(),
                          ui->lab_time_end->text().toUtf8().constData());

            }
#if 1
            else //下载
            {
                play_status = EM_PLAY_STATUS_RUNNING;
                DBG_PRINT("file download start, chn: %d\n", playback_chn);
            }
#endif
        } break;

        case EM_BIZ_EVENT_PLAYBACK_RUN:
        {
            u32 cur_pos = playback_msg.stream_progress.cur_pos;
            u32 total_size = playback_msg.stream_progress.total_size;

            if (playback_chn < 0x10) //回放
            {
                DBG_PRINT("cur_pos: %u, total_size: %u, %d\n", cur_pos, total_size, play_file.end_time - play_file.start_time);

                play_pos = cur_pos;
                u32 cur_time = play_file.start_time + cur_pos;
                u32 end_time = play_file.end_time;
                struct tm tm_time;

                if (cur_time > end_time)
                {
                    ERR_PRINT("start_time >= end_time\n");

                    return;
                }

                cur_time += 8*3600;
                end_time += 8*3600;

                gmtime_r((time_t *)&cur_time, &tm_time);
                QTime qtime_cur(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

                gmtime_r((time_t *)&end_time, &tm_time);
                QTime qtime_end(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
                ui->lab_time_end->setText(qtime_cur.toString(QString::fromUtf8("HH:mm:ss"))
                                          + QString::fromUtf8("/")
                                          + qtime_end.toString(QString::fromUtf8("HH:mm:ss")));//play_cur_time/play_end_time


                if (!b_slider_mover
                        && EM_PLAY_STATUS_STOP != play_status)
                {
                    //if (total_size != ui->slider_play->maximum())
                    {
                        //ERR_PRINT("total_size(%u) != ui->slider_play->maximum(%u)\n", total_size, ui->slider_play->maximum());
                    }
                    ui->slider_play->setValue(play_pos);
                    DBG_PRINT("update progress pos: %d\n", play_pos);
                }
            }
            else // 下载
            {
                //进度
                DBG_PRINT("cur_pos: %u, total_size: %u\n", cur_pos, total_size);
            }
        } break;

        case EM_BIZ_EVENT_PLAYBACK_DONE:
        {
            DBG_PRINT("playback done\n");
            play_status = EM_PLAY_STATUS_STOP;

            if (playback_chn < 0x10) //回放
            {
                //播放控制
                //btn play/pause
                ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_play.bmp")));
            }
            else // 下载
            {

            }
            BizPlaybackStop(playback_chn);

        } break;

        case EM_BIZ_EVENT_PLAYBACK_NETWORK_ERR:
        {
            s32 stream_errno = playback_msg.stream_errno;
            play_status = EM_PLAY_STATUS_STOP;

            DBG_PRINT("stream_errno: %d\n", stream_errno);
            BizPlaybackStop(playback_chn);
        } break;

        default:
            ERR_PRINT("type: %d, not support\n", type);
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

    //showTableWidget(true);
    ui->btn_extra->setChecked(true);
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

void form_playback::on_btn_backup_clicked()
{
    if (INVALID_VALUE == file_down_index)
    {
        ShowMessageBoxInfo(QString::fromUtf8("请先在搜索文件列表框中选定一个文件！"));
        return ;
    }
    //DBG_PRINT("file_down_index: %d\n", file_down_index);

    u32 start_time = 0;
    u32 end_time = 0;

    start_time = search_result.pfile_info[file_down_index].start_time;
    end_time = search_result.pfile_info[file_down_index].end_time;
    start_time += 8*3600;
    end_time += 8*3600;

    struct tm tm_time;
    gmtime_r((time_t *)&start_time, &tm_time);
    QTime qtime_start(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

    gmtime_r((time_t *)&end_time, &tm_time);
    QTime qtime_end(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

    QString msg = QString(QString::fromUtf8("%1-%2 确认下载该文件?").arg(qtime_start.toString(QString::fromUtf8("HH:mm:ss"))).arg(qtime_end.toString(QString::fromUtf8("HH:mm:ss"))));
    int ret = ShowMessageBoxQuesion(msg);
    if (0 == ret)
    {
        return ;
    }

#if 1
    ret = SUCCESS;
    DBG_PRINT("offset: %u, size: %u\n", search_result.pfile_info[file_down_index].offset, search_result.pfile_info[file_down_index].size);
    ret = BizDownloadByFile(search_nvr_ip, &search_result.pfile_info[file_down_index]);
    if (ret)
    {
        ERR_PRINT("BizDownloadByFile failed, ret: %d\n", ret);
    }
#endif

#if 0
    DialogProgress dialog;
    dialog.setTitle(QString::fromUtf8("文件下载"));
    connect;

    ret = dialog.exec();
    DBG_PRINT("dialog return: %d\n", ret);
#endif


}

//play ctl
void form_playback::on_btn_stop_clicked()
{

    int ret = SUCCESS;

    if (EM_PLAY_STATUS_STOP == play_status)
    {
        //ShowMessageBoxInfo(QString::fromUtf8("请双击文件进行播放"));
        return ;
    }

    ret = BizPlaybackStop(0);
    if (ret)
    {
        ERR_PRINT("BizPlaybackStartByFile failed, ret: %d\n", ret);
    }
    play_status = EM_PLAY_STATUS_STOP;

}

void form_playback::on_btn_play_clicked()
{
    int ret = SUCCESS;

    DBG_PRINT("play status: %d\n", play_status);

    switch (play_status)
    {
        case EM_PLAY_STATUS_STOP:
        {
            ShowMessageBoxInfo(QString::fromUtf8("请双击文件进行播放"));
        } break;

        case EM_PLAY_STATUS_RUNNING:
        {
            ret = BizPlaybackPause(0);
            if (ret)
            {
                ERR_PRINT("BizPlaybackPause failed, ret: %d\n", ret);
            }
            else
            {
                play_status = EM_PLAY_STATUS_PAUSE;
                ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_pause.bmp")));
            }
        } break;

        case EM_PLAY_STATUS_PAUSE:
        case EM_PLAY_STATUS_STEP:
        {
            ret = BizPlaybackResume(0);
            if (ret)
            {
                ERR_PRINT("BizPlaybackResume failed, ret: %d\n", ret);
            }
            else
            {
                play_status = EM_PLAY_STATUS_RUNNING;
                ui->btn_play->setIcon(QIcon(QString::fromUtf8(":/image/pctl_play.bmp")));
            }
        } break;

    default:
        ERR_PRINT("play_status(%d) not support\n", play_status);
    }
}

void form_playback::on_btn_step_clicked()
{
    int ret = SUCCESS;

    DBG_PRINT("play status: %d\n", play_status);

    switch (play_status)
    {
        case EM_PLAY_STATUS_STOP:
        {
            //ShowMessageBoxInfo(QString::fromUtf8("请双击文件进行播放"));
        } break;

        case EM_PLAY_STATUS_RUNNING:
        {
            ShowMessageBoxInfo(QString::fromUtf8("请先暂停播放"));
        } break;

        case EM_PLAY_STATUS_PAUSE:
        {
            play_status = EM_PLAY_STATUS_STEP;
        } //break;

        case EM_PLAY_STATUS_STEP:
        {
            ret = BizPlaybackStep(0);
            if (ret)
            {
                ERR_PRINT("BizPlaybackStep failed, ret: %d\n", ret);
            }
        } break;

    default:
        ERR_PRINT("play_status(%d) not support\n", play_status);
    }
}

void form_playback::set_play_rate(int index)
{
    int ret = SUCCESS;

    if (EM_PLAY_STATUS_RUNNING != play_status)
    {
        ShowMessageBoxInfo(QString::fromUtf8("回放速率只能在回放状态下操作"));
        ui->cmb_rate->setCurrentIndex(play_rate_index);
        return ;
    }

    int rate_convert[] = {-8, -4, -2, 1, 2, 4, 8};
    if (index < 0 || index > 6)
    {
        ERR_PRINT("index(%d) invalid, [0, 6]\n", index);
        ui->cmb_rate->setCurrentIndex(play_rate_index);
        return ;
    }

    int rate = rate_convert[index];
    ret = BizPlaybackRate(0, rate);
    if (ret)
    {
        ERR_PRINT("BizPlaybackRate(%d) failed, ret: %d\n", rate, ret);
        ui->cmb_rate->setCurrentIndex(play_rate_index);
    }
    else
    {
        play_rate_index = index;
    }
}

void form_playback::slider_pressed(int pos)
{
    if (EM_PLAY_STATUS_RUNNING != play_status)
    {
        ShowMessageBoxInfo(QString::fromUtf8("拖动定位只能在回放状态下操作"));
        return ;
    }

    b_slider_mover = true;
    slider_pressed_pos = pos;
    DBG_PRINT("pos: %d\n", pos);

    u32 label_time = play_file.start_time + pos;
    label_time += 8*3600;
    struct tm tm_time;
    gmtime_r((time_t *)&label_time, &tm_time);
    QTime qtime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    m_displayLabel->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
    m_displayLabel->adjustSize();

    int g_x = (double)pos / (ui->slider_play->maximum() - ui->slider_play->minimum()) * ui->slider_play->width() + ui->slider_play->mapToGlobal(QPoint(0,0)).x();
    g_x -= m_displayLabel->width()/2;
    int g_y = ui->slider_play->mapToGlobal(QPoint(0,0)).y()-m_displayLabel->height();
    QPoint pot = this->mapFromGlobal(QPoint(g_x, g_y));
    m_displayLabel->move(pot);
    m_displayLabel->setVisible(true);
    //DBG_PRINT("label x: %d, y: %d, g_x: %d, g_y: %d\n", pot.x(), pot.y(), g_x, g_y);

}

void form_playback::slider_moved(int pos)
{
    if (EM_PLAY_STATUS_RUNNING != play_status)
    {
        ShowMessageBoxInfo(QString::fromUtf8("拖动定位只能在回放状态下操作"));
        return ;
    }

    //DBG_PRINT("slider cur_value: %d, value: %d\n", ui->slider_play->value(), pos);

    u32 label_time = play_file.start_time + pos;
    label_time += 8*3600;
    struct tm tm_time;
    gmtime_r((time_t *)&label_time, &tm_time);
    QTime qtime(tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    m_displayLabel->setText(qtime.toString(QString::fromUtf8("HH:mm:ss")));
    m_displayLabel->adjustSize();

    int g_x = (double)pos / (ui->slider_play->maximum() - ui->slider_play->minimum()) * ui->slider_play->width() + ui->slider_play->mapToGlobal(QPoint(0,0)).x();
    g_x -= m_displayLabel->width()/2;
    int g_y = ui->slider_play->mapToGlobal(QPoint(0,0)).y()-m_displayLabel->height();
    QPoint pot = this->mapFromGlobal(QPoint(g_x, g_y));
    m_displayLabel->move(pot);
    //DBG_PRINT("label x: %d, y: %d, g_x: %d, g_y: %d\n", pot.x(), pot.y(), g_x, g_y);
}

void form_playback::slider_released(int pos)
{
    int ret = SUCCESS;

    if (EM_PLAY_STATUS_RUNNING != play_status)
    {
        ShowMessageBoxInfo(QString::fromUtf8("拖动定位只能在回放状态下操作"));
        return ;
    }

     b_slider_mover = false;
     m_displayLabel->setVisible(false);
     DBG_PRINT("slider pressed_pos: %d, releaseed_pos: %d\n", slider_pressed_pos, pos);

     if (pos > (play_file.end_time - play_file.start_time) )
     {
         ERR_PRINT("pos(%d) out of range[0, %d]\n", pos, play_file.end_time - play_file.start_time);
         return ;
     }

     u32 seek_time = pos;
     seek_time *= 1000; //ms
     ret = BizPlaybackSeek(0, seek_time);
     if (ret)
     {
         ERR_PRINT("BizPlaybackSeek failed, ret: %d\n", ret);
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

void form_playback::showEvent ( QShowEvent * event )
{
    BizEnterPlayback();
}

void form_playback::hideEvent ( QHideEvent * event )
{
    BizLeavePlayback();
}




