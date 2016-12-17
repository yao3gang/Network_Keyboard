#include <time.h>

#include <QtGui>
#include "form_playback.h"
#include "ui_form_playback.h"
#include "frmmessagebox.h"

#include "page_manager.h"
#include "page_main.h"
#include "page_dev_mgt.h"

#include "biz_system_complex.h"

#define MAX_FILE_NUMS (24)

form_playback::form_playback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_playback)
{
    memset(&search_para, 0, sizeof(ifly_recsearch_param_t));

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

    //btn extra
    ui->btn_extra->setIcon(QIcon(QString::fromUtf8(":/image/up.bmp")));
    ui->btn_extra->setToolTip(QString::fromUtf8("显示文件列表"));

    ui->btn_extra->setCheckable(true);
    ui->btn_extra->setAutoDefault(false);

    ui->widget_result->hide();

    connect(ui->btn_extra, SIGNAL(toggled(bool)), this, SLOT(showTableWidget(bool)));
}

void form_playback::showTableWidget(bool b)
{
    ui->widget_result->setVisible(b);

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
    QList<QTreeWidgetItem *> list_item;

    in.s_addr = dev.deviceIP;
    qstr_ip = QString(inet_ntoa(in));
    if (qstr_ip.isEmpty())
    {
        ERR_PRINT("dev ip invalid\n");
        return ;
    }
    DBG_PRINT("dev type: %d, ip: %s\n", dev.devicetype, qstr_ip.toUtf8().constData());

    if (dev.devicetype == EM_NVR)
    {
        list_item = ui->treeWidget_nvr->findItems(qstr_ip, Qt::MatchContains);

        if (list_item.size() != 1)//查找结果必须 == 1
        {
            DBG_PRINT("findItems size(%d) != 1, dev ip: %s\n", list_item.size(), qstr_ip.toUtf8().constData());

            return ;
        }
        QTreeWidgetItem *item = list_item.at(0);

        ui->treeWidget_nvr->deleteItemChildren(item);//销毁所有通道条目

        if (dev.b_alive)
        {
            item->setIcon(0, QIcon(":/image/dev_online.png"));

            //设备在线 添加所有通道条目
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
                item->addChildren(list_chn);
            }
        }
        else
        {
            item->setIcon(0, QIcon(":/image/dev_offline.png"));
        }
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

void form_playback::on_btn_srh_clicked()
{
    memset(&search_para, 0, sizeof(ifly_recsearch_param_t));

    //get nvr ip & chn
    QTreeWidgetItem *item = ui->treeWidget_nvr->currentItem();
    if (NULL == item)
    {
        ERR_PRINT("current item == NULL\n");

        ShowMessageBoxInfo(QString::fromUtf8("请选定NVR的一个通道！"));
        return ;
    }

    if (NULL == item->parent())
    {
        ERR_PRINT("current item parent == NULL\n");

        ShowMessageBoxInfo(QString::fromUtf8("请选定NVR的一个通道！"));
        return ;
    }

    QString str_nvr_ip = item->parent()->text(0);
    u32 nvr_ip = inet_addr(str_nvr_ip.toUtf8().constData());
    if (INADDR_NONE == nvr_ip)
    {
        ERR_PRINT("nvr ip invalid, str_nvr_ip: %s\n", str_nvr_ip.toUtf8().constData());

        ShowMessageBoxError(QString::fromUtf8("选定的NVR IP地址无效！"));
        return ;
    }

    //req nvr chn
    QString str_chn = item->text(0);
    if (!str_chn.contains(QString::fromUtf8("通道"), Qt::CaseInsensitive))//不含有"通道"
    {
        ERR_PRINT("chn str: %s, contains invalid\n", str_chn.toUtf8().constData());

        ShowMessageBoxError(QString::fromUtf8("选定的NVR通道有误！"));
        return ;
    }

    u8 nvr_chn = 0xff;
    if (1 != sscanf(str_chn.toUtf8().constData(), "通道%d", &nvr_chn))
    {
        ERR_PRINT("chn str: %s, sscanf invalid\n", str_chn.toUtf8().constData());

        ShowMessageBoxError(QString::fromUtf8("选定的NVR通道有误！"));
        return ;
    }
    nvr_chn -= 1;

    //date & time
    u32 start_time = 0;
    u32 end_time = 0;
    struct tm tmtemp;
    QDate qd = ui->dateEdit->date();

    tmtemp.tm_year = qd.year() -1900;
    tmtemp.tm_mon = qd.month() -1;
    tmtemp.tm_mday = qd.day();

    QTime qt = ui->timeEdit_start->time();
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

    int ret = BizDevRecFilesSearch(nvr_ip, &search_para, &search_result);
    if (ret)
    {
        ERR_PRINT("BizGetDevChnName failed, ret: %d\n", ret);
    }

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
}

